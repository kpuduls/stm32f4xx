#include "main.h"

/* Main loop */
int main(void)
{
	init();

	dma1_start_stream5((uint32_t)(&USART2->DR), (uint32_t)&rx_dma_buffer, RX_DMA_BUFFER_SIZE);

    while(1)
    {
    }

    return 0;
}

/* User functions */
void init(void)
{
	/* Initialise the gpio led, uart2 rxtx dma modules */
	gpioa_led_init();
    uart2_txrx_init();
    dma1_stream6_init();
    dma1_stream5_init();
}
