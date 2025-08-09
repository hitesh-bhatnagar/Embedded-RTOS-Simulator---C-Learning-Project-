// What is semaphore ?
//  It's a signal based locking mech. that helps coordinate access to shared resource or condition between tasks.

//  Ex: 
// Task A -> waits for sensor data
//  ISR or TASK B -> posts to the semaphore 
//  Now TASK A can proceed

// Ex 2 : 2 tasks need to access UART(shared resource)
//     Semaphore ensures only one task uses it at a time(mutex - style)

//     Function                 Role

// Semaphore struct  ===>       Holds count and a waiting task list
// SemaphoreInit()   ===>       Initializes a semaphore with count
// semaphoreWait()   ===>       Called by a task that wants to use resource
// semaphoreSignal() ===>       Called when the resource is released (or free again)

#include <stdio.h>
#include <string.h>
#include "semaphore.h"

void semaphoreInit(semaphore *s, int initialCount){
    s -> count = initialCount;
}

void semaphoreWait(semaphore *s, Task *t){
    if(s -> count > 0) s->count--;
    else{
        printf("[SEMAPHORE] Task %s is blocked waiting\n", t -> taskName);
        t -> state = TASK_BLOCKED;
    }
}

void semaphoreSignal(semaphore *s){
    s -> count++;   
    // wake up a blocked task if any 
    for(int i = 0; i < taskCount; i++){
        if(taskList[i].state == TASK_BLOCKED){
            taskList[i].state = TASK_READY;
            printf("[SEMAPHORE] Waking up task: %s\n", taskList[i].taskName);
            break;
        }
    }
}