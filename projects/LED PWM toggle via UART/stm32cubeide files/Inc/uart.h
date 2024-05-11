#ifndef UART_H_
#define UART_H_
#include "stm32f4xx.h"
#include "stdint.h"


void uart1_rxtx_init(void);
void uart1_write(int ch);
char uart1_read(void);

#endif
