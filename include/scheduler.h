#ifndef SCHEDULER_H
#define SCHEDULER_H
extern int currentTaskIndex;

void runScheduler();
void tick();
int getTickCounter();
int getCurrentTaskId(); 

#endif

