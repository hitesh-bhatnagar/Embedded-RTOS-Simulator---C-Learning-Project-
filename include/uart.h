#ifndef UART_H
#define UART_H
#define UART_BUFFER_SIZE 64

typedef struct{
    char buffer[UART_BUFFER_SIZE];
    int head, tail, size;   // these for circular buffer 
} UARTBuffer;

void UART_init();
int UART_send(char c); // will send to TX
int  UART_receive(char *c); // will read from RX
void UART_isr();        // will transfer from tx to rx

#endif