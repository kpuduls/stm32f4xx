#ifndef UART_DMA_H_
#define UART_DMA_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include <stdint.h>

#define RX_DMA_BUFFER_SIZE 128
#define RX_DATA_BUFFER_SIZE  512

extern char rx_dma_buffer[RX_DMA_BUFFER_SIZE];
extern uint32_t dma_head;
extern uint32_t dma_tail;

extern char rx_data_buffer[RX_DATA_BUFFER_SIZE];
extern uint32_t data_head;
extern uint32_t data_tail;

void update_crc_buffer_pointer(uint32_t *pointer, uint32_t buffer_size, uint32_t increment);
void copyBuffer(char *src, char *dst, uint32_t src_head, uint32_t src_tail, uint32_t dst_head, uint32_t dst_tail);;
void dma1_stream6_init(void);
void dma1_start_stream6(uint32_t src, uint32_t dst, uint32_t len);
void dma1_stream5_init(void);
void dma1_start_stream5(uint32_t src, uint32_t rx_dma_buffer, uint32_t buffer_size);
void uart2_tx_init(void);
void uart2_txrx_init(void);
void DMA1_Stream5_IRQHandler(void);
void DMA1_Stream6_IRQHandler(void);

#endif /* UART_DMA_H_ */
