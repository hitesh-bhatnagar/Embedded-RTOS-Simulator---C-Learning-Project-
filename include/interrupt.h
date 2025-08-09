#ifndef INTERRUPT_H
#define INTERRUPT_H

#define MAX_INTERRUPTS 10

typedef void(*isr_Handler)();

void registerInterrupt(int id, isr_Handler handler);
void triggerInterrupt(int id);

#endif