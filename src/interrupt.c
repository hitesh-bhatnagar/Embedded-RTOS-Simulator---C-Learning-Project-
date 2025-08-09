#include <stdio.h>
#include "interrupt.h"


isr_Handler isrTable[MAX_INTERRUPTS] = {0};

void registerInterrupt(int id, isr_Handler handler){
    if(id >= 0 && id < MAX_INTERRUPTS) isrTable[id] = handler;

}

void triggerInterrupt(int id){
    if(id >= 0 && id < MAX_INTERRUPTS && isrTable[id]){
        printf("[INTERRUPT] Triggering interrupt ID: %d\n", id);
        isrTable[id]();
    }else{
        printf("[INTERRUPT] Invalid or unregistered interrupt ID: %d\n", id);
    }
}