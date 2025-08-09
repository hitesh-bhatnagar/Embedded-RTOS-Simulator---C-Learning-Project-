#ifndef TIMER_H
#define TIMER_H

void timer_isr();
void init_timer();
void register_Timer_callback(void (*callback)(void));

#endif