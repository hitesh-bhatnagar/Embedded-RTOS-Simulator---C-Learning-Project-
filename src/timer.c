#include <stdio.h>
#include "timer.h"
#include "utils.h"

static void (*timer_callback)(void) = 0;

void init_timer(){
    timer_callback = 0;
}

void register_Timer_callback(void (*callback)(void)){
    timer_callback = callback;

}

void timer_isr(){
    printf("[TIMER ISR] Timer overflow triggered !!!\n");

    if(timer_callback) timer_callback();    // calling user defined callback (like in 8051);
}