#ifndef TASK_H
#define TASK_H
#define MAX_TASKS 10

typedef enum{
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_SLEEPING,
    TASK_TERMINATED
} TaskState;

typedef struct {
    int taskId;
    char taskName[50];
    TaskState state;
    int priority;
    void (*taskFunction)(void);
    int sleepTicks;
} Task;

extern Task taskList[MAX_TASKS];
extern int taskCount;

void createTask(void (*taskFunction)(void), const char* name, int priority);
const char* getTaskStateName(TaskState s);
void yield();
void taskSleep(int ticks);
#endif
