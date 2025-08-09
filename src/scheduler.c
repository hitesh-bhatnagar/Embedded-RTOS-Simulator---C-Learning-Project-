/*
runScheduler()
    runs the next task in round-robin fasion
    skips tasks that are not ready

yield()
    a process that is currently running must explicitly give up the CPU before another process can run


// Below is implementation of ROUND-ROBIN

void runScheduler()
{
    int highest = -1, next = -1;
    static int last = -1;

    // find highest READY priority
    for (int i = 0; i < taskCount; i++)
        if (taskList[i].state == TASK_READY && taskList[i].priority > highest)
            highest = taskList[i].priority;

    // round-robin among those
    for (int i = 1; i <= taskCount; i++)
    {
        int idx = (last + i) % taskCount;
        if (taskList[idx].state == TASK_READY && taskList[idx].priority == highest)
        {
            next = idx;
            break;
        }
    }

    if (next != -1)
    {
        last = next;
        taskList[next].state = TASK_RUNNING;
        currentTaskId = next;
        printTimestamp();
        printf(CLR_CYAN "[SCHEDULER] " CLR_RESET "Running: %s (Priority = %d)\n",
               taskList[next].taskName, taskList[next].priority);

        taskList[next].taskFunction();
        if (taskList[next].state == TASK_RUNNING)
            taskList[next].state = TASK_READY;
    }
    else
    {
        // no READY task
    }
}
*/

#include "task.h"
#include "scheduler.h"
#include "timer.h"
#include <stdio.h>
#include <time.h>
#include "utils.h"



extern Task taskList[];
extern int taskCount;

static int lastSelected = -1;
static int tickCounter = 0;

static int currentTaskId = -1;
int currentTaskIndex = -1; 

/*
    Now Implementing preemptive priority based scheduler

    Everytime when call runScheduler(), it:
        1. Scans for the highest priority among READY tasks.
        2. Round robins amount tasks with that priority.
        3. Marks it RUNNING, calls its function then if still RUNNING reverts to READY
*/

void runScheduler()
{
    int highestPrio = -1;

    // 1) Find the maximum priority among READY tasks
    for (int i = 0; i < taskCount; i++)
    {
        if (taskList[i].state == TASK_READY &&
            taskList[i].priority > highestPrio)
        {
            highestPrio = taskList[i].priority;
        }
    }

    // 2) Round‐robin pick among those with highestPrio
    int selected = -1;
    for (int i = 1; i <= taskCount; i++)
    {
        int idx = (lastSelected + i) % taskCount;
        if (taskList[idx].state == TASK_READY &&
            taskList[idx].priority == highestPrio)
        {
            selected = idx;
            break;
        }
    }

    // 3) If found, run it
    if (selected != -1)
    {
        lastSelected = selected;
        currentTaskId = selected;
        taskList[selected].state = TASK_RUNNING;

        printf("[SCHEDULER] Running: %s (Priority: %d)\n",
               taskList[selected].taskName,
               taskList[selected].priority);

        // Execute the task
        taskList[selected].taskFunction();

        // If it never blocked or terminated itself, set back to READY
        if (taskList[selected].state == TASK_RUNNING)
        {
            taskList[selected].state = TASK_READY;
        }
    }
}

/*
  tick() simulates your system tick:
        Increments tickCounter
        Wakes sleeping tasks after their sleepTicks elapse
        Re‐enables the shell task every 4 ticks
        Calls the timer ISR every 5 ticks
 */

void tick()
{
    tickCounter++;

    // Wake up any sleeping tasks
    for (int i = 0; i < taskCount; i++)
    {
        if (taskList[i].state == TASK_SLEEPING &&
            --taskList[i].sleepTicks <= 0)
        {
            taskList[i].state = TASK_READY;
            printf("[WAKE] %s is now READY\n", taskList[i].taskName);
        }
    }

    // Shell task assumed to be ID=3; awaken every 4 ticks if blocked
    if (tickCounter % 4 == 0 &&
        taskList[3].state == TASK_BLOCKED)
    {
        taskList[3].state = TASK_READY;
    }

    // Simulate a hardware timer interrupt every 5 ticks
    if (tickCounter % 5 == 0)
    {
        timer_isr();
    }
}

int getTickCounter() { return tickCounter; }
int getCurrentTaskId() { return currentTaskId; }