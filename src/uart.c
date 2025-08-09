#include <stdio.h>
#include "uart.h"
#include "utils.h"

UARTBuffer txBuffer;
UARTBuffer rxBuffer;

void UART_init(){
    txBuffer.head = txBuffer.tail = txBuffer.size = 0;
    rxBuffer.head = rxBuffer.tail = rxBuffer.size = 0;
}

int UART_send(char c){
    if(txBuffer.size >= UART_BUFFER_SIZE) return -1;    // buffer full
    txBuffer.buffer[txBuffer.tail] = c;
    txBuffer.tail = (txBuffer.tail + 1) % UART_BUFFER_SIZE;
    txBuffer.size++;
    return 0;
}

int UART_receive(char *c){
    if(rxBuffer.size == 0) return -1;   // no data
    *c = rxBuffer.buffer[rxBuffer.head];
    rxBuffer.head = (rxBuffer.head + 1) % UART_BUFFER_SIZE;
    rxBuffer.size--;
    return 0;
}

void UART_isr(){
    if(txBuffer.size > 0 && rxBuffer.size < UART_BUFFER_SIZE){
        char c = txBuffer.buffer[txBuffer.head];
        txBuffer.head = (txBuffer.head + 1) % UART_BUFFER_SIZE;
        txBuffer.size--;

        rxBuffer.buffer[rxBuffer.tail] = c;
        rxBuffer.tail = (rxBuffer.tail + 1) % UART_BUFFER_SIZE;
        rxBuffer.size++;

        printf("[UART] Transmitted '%c' from Tx to Rx\n", c);
    }
    else printf("Nothing to transmit or receive");
}