#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include "task.h"

typedef struct{
    int count;
} semaphore;

void semaphoreInit(semaphore *s, int initialCount);
void semaphoreWait(semaphore *s, Task *t);
void semaphoreSignal(semaphore *s);


#endif