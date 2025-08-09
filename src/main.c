#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "task.h"
#include "scheduler.h"
#include "ipc.h"
#include "uart.h"
#include "semaphore.h"
#include "interrupt.h"
#include "timer.h"
#include "utils.h"

#define MAX_LOG 50
#define CPU_INTERVAL 10

MessageQueue queue;
semaphore uartSem;
char logBuffer[MAX_LOG][64];
int logIndex = 0;

static int total_ticks = 0, idle_ticks = 0; // CPU usage counters

void task_networkListener();
void task_logger();
void task_LED();
void task_shell();
void task_idle();

void task_idle(){
    idle_ticks++;
    printf("[IDLE] CPU is idle\n");
    taskSleep(1);
}

// interrupt handler
void buttonISR()
{
    char *m = strdup("INTERRUPT: Button pressed!!!");
    sendMessage(&queue, m);
    taskList[1].state = TASK_READY; // wake logger
    printf("[INT] Button ISR\n");
}

// timer callback
void systemCallback()
{
    printf("[TIMER] Tick %d\n", getTickCounter());
    // wake LED if blocked
    for (int i = 0; i < taskCount; i++)
        if (!strcmp(taskList[i].taskName, "LED_Blinker") && taskList[i].state == TASK_BLOCKED)
        {
            taskList[i].state = TASK_READY;
            printf("[TIMER] Woke LED\n");
        }
}

// Task implementations
void task_networkListener()
{
    static int cnt = 0;
    char buf[64];
    snprintf(buf, sizeof(buf), "Net Event #%d", ++cnt);
    char *m = strdup(buf);
    if (sendMessage(&queue, m) != 0)
    {
        free(m);
        printf("[NET] Queue full\n");
    }
    else
    {
        printTimestamp();
        printf(CLR_GREEN "[NET] " CLR_RESET "Sent: %s\n", buf);
    }
    taskSleep(3);
}

void task_logger()
{
    void *v;
    if (receiveMessage(&queue, &v) == 0)
    {
        char *s = v;
        printTimestamp();
        printf(CLR_YELLOW "[LOGGER] " CLR_RESET "%s\n", s);

        if (logIndex < MAX_LOG)
            strncpy(logBuffer[logIndex++], s, 64);
        free(s);
    }
    else
    {
        printf("[LOGGER] (no msg)\n");
    }
    taskSleep(1);
}

void task_LED()
{
    static int st = 0;
    st = !st;
    printTimestamp();
    printf(CLR_MAGENTA "[LED] " CLR_RESET "State: %s\n", st ? "ON" : "OFF");

    taskSleep(5);
}

void task_shell()
{
    char cmd[128];
    printTimestamp();
    printf(CLR_BLUE "[SHELL] >>> " CLR_RESET);
    fflush(stdout);

    if (fgets(cmd, sizeof(cmd), stdin))
    {
        cmd[strcspn(cmd, "\n")] = 0;
        if (!strcmp(cmd, "help"))
        {
            puts(
                CLR_CYAN "Available commands:\n" CLR_RESET
                         "  help                   - Show this help menu\n"
                         "  list                   - List all tasks\n"
                         "  send <message>         - Send message to logger\n"
                         "  log show|clear         - View or clear the log buffer\n"
                         "  status                 - Show system status table\n"
                         "  trigger <taskId>       - Force a task to READY\n"
                         "  trigger interrupt <id> - Fire software ISR\n"
                         "  clear                  - Clear the screen\n");
        }

        else if (!strcmp(cmd, "list"))
        {
            for (int i = 0; i < taskCount; i++)
                printf("T[%d] %s %s Prio=%d\n",
                       taskList[i].taskId,
                       taskList[i].taskName,
                       getTaskStateName(taskList[i].state),
                       taskList[i].priority);
        }
        else if (!strncmp(cmd, "send ", 5))
        {
            char *m = strdup(cmd + 5);
            if (sendMessage(&queue, m) == 0)
                printf("[SHELL] queued\n");
            else
            {
                free(m);
                printf("[SHELL] full\n");
            }
        }
        else if (!strcmp(cmd, "log show"))
        {
            if (!logIndex)
                puts("[SHELL] log empty");
            else
                for (int i = 0; i < logIndex; i++)
                    printf("  [%d] %s\n", i, logBuffer[i]);
        }
        else if (!strcmp(cmd, "clear"))
        {
            printf("\033[2J\033[H");
        }
        else if (!strcmp(cmd, "status"))
        {
            int r = 0, b = 0, term = 0;
            for (int i = 0; i < taskCount; i++)
            {
                if (taskList[i].state == TASK_READY)
                    r++;
                if (taskList[i].state == TASK_BLOCKED)
                    b++;
                if (taskList[i].state == TASK_TERMINATED)
                    term++;
            }
            printTimestamp();
            printf(CLR_BLUE "┌────────┬───────┬────────┐\n" CLR_RESET);
            printf(CLR_BLUE "│ Tasks  │ Queue │ Logs   │\n" CLR_RESET);
            printf(CLR_BLUE "├────────┼───────┼────────┤\n" CLR_RESET);
            printf(CLR_BLUE "│ %6d │ %5d │ %6d │\n" CLR_RESET,
                   taskCount, queue.size, logIndex);
            printf(CLR_BLUE "└────────┴───────┴────────┘\n" CLR_RESET);
        }

        else if (!strncmp(cmd, "trigger interrupt ", 18))
        {
            int id = atoi(cmd + 18);
            triggerInterrupt(id);
        }
        else if (!strncmp(cmd, "trigger ", 8))
        {
            int id = atoi(cmd + 8);
            if (id >= 0 && id < taskCount)
            {
                taskList[id].state = TASK_READY;
                printf("[SHELL] T[%d] READY\n", id);
            }
        }
        else
        {
            printf("[SHELL] unkown command\n");
        }
    }
    // block itself until tick wakes it
    taskList[3].state = TASK_BLOCKED;
    taskSleep(2);
}

int main()
{

    printf("==== RTOS Boot ====\n");

    initQueue(&queue);

    UART_init();
    semaphoreInit(&uartSem, 1);
    init_timer();

    register_Timer_callback(systemCallback);
    registerInterrupt(0, buttonISR);

    createTask(task_networkListener, "NetListener", 1);
    createTask(task_logger, "Logger", 1);
    createTask(task_LED, "LED_Blinker", 1);
    createTask(task_shell, "ShellTask", 1);
    createTask(task_idle, "IdleTask", 0);

    int tickCount = 0;

    while (1)
    {
        printf("\n[TICK %d]\n", tickCount++);

        tick();
        runScheduler();
        
        total_ticks++;
        if(total_ticks % CPU_INTERVAL == 0){
            float usage = 100.0f * (total_ticks - idle_ticks) / total_ticks;
            printf("[SYSTEM] CPU usage : %.2f%% (%d/%d ticks busy)\n", usage, total_ticks - idle_ticks, total_ticks);
        }

        fflush(stdout);
        usleep(1000000);    // 1sec delay
    }
    return 0;
}
