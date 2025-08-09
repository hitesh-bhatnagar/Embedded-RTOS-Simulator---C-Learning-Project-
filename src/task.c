// task.c
#include <stdio.h>
#include <string.h>
#include "task.h"
#include "scheduler.h"
#include "utils.h"

#define MAX_TASKS 10

Task taskList[MAX_TASKS];
int taskCount = 0;
extern int currentTaskIndex;

void createTask(void (*taskFunction)(void), const char *name, int priority)
{
    if (taskCount >= MAX_TASKS)
    {
        printf("[ERROR] Task limit reached !!!!\n");
        return;
    }

    Task *task = &taskList[taskCount];

    task->taskId = taskCount;
    strncpy(task->taskName, name, sizeof(task->taskName) - 1);
    task->taskName[sizeof(task->taskName) - 1] = '\0';
    task->taskFunction = taskFunction;
    task->state = TASK_READY; 
    task->priority = priority;
    task->sleepTicks = 0;

    printf("[KERNEL] Task Created: ID=%d | Name=%s | Priority=%d\n",
           task->taskId, task->taskName, task->priority);

    taskCount++;
}

const char *getTaskStateName(TaskState s)
{
    switch (s)
    {
    case TASK_READY:
        return "READY";
    case TASK_RUNNING:
        return "RUNNING";
    case TASK_BLOCKED:
        return "BLOCKED";
    case TASK_SLEEPING:
        return "SLEEPING";
    case TASK_TERMINATED:
        return "TERMINATED";
    default:
        return "UNKNOWN";
    }
}

void yield()
{
    runScheduler();
}

void taskSleep(int ticks)
{
    if (currentTaskIndex >= 0)
    {
        taskList[currentTaskIndex].state = TASK_SLEEPING;
        taskList[currentTaskIndex].sleepTicks = ticks;
        printf("[SLEEP] Task %s sleeping for %d ticks\n", taskList[currentTaskIndex].taskName, ticks);
    }
}
