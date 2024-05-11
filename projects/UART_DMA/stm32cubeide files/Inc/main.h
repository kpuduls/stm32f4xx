#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart_dma.h"
#include "gpioa_led.h"


/* User defined function declarations */
void init(void);
void DMA1_Stream6_IRQHandler(void);
void DMA1_Stream5_IRQHandler(void);

#endif /* MAIN_H_ */
