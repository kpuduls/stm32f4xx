#include "uart_dma.h"

#define SYS_FREQ		           16000000
#define APB1_CLK		           SYS_FREQ
#define UART_BAUDRATE	           115200

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);

char rx_dma_buffer[] = {};
uint32_t dma_head = (uint32_t)0;
uint32_t dma_tail = (uint32_t)0;

char rx_data_buffer[] = {};
uint32_t data_head = (uint32_t)0;
uint32_t data_tail = (uint32_t)0;

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate) {
    USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate) {
    return ((PeriphClk + (BaudRate / 2U)) / BaudRate);
}

void uart2_tx_init(void) {
    /* Enable clock for GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Configure GPIOA pin 2 as alternate function mode (AF) */
    GPIOA->MODER &= ~(GPIO_MODER_MODER2_0);  // Reset bit 0
    GPIOA->MODER |= GPIO_MODER_MODER2_1;     // Set bit 1

	/*Set PA2 alternate function type to UART_TX (AF07)*/
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

    /* Enable clock for UART2 */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* Configure UART2 baud rate */
    uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

    /* Enable UART2 transmitter and UART2 */
    USART2->CR1 |= USART_CR1_TE;             // Enable transmitter
    USART2->CR1 |= USART_CR1_UE;             // Enable USART2
}

void uart2_txrx_init(void) {
    /* Enable clock for GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Configure GPIOA pin 2,3 as alternate function mode (AF) */
    GPIOA->MODER &= ~((GPIO_MODER_MODER3_0) | (GPIO_MODER_MODER2_0));  // Reset bit 0
    GPIOA->MODER |= (GPIO_MODER_MODER3_1 | GPIO_MODER_MODER2_1);     // Set bit 1

	/*Set PA2, PA3 alternate function type to UART_TXRX (AF07)*/
	GPIOA->AFR[0] |= ((1U<<12) | (1U<<8));
	GPIOA->AFR[0] |= ((1U<<13) | (1U<<9));
	GPIOA->AFR[0] |= ((1U<<14) | (1U<<10));
	GPIOA->AFR[0] &= ~((1U<<15) | (1U<<11));

    /* Enable clock for UART2 */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* Configure UART2 baud rate */
    uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

    /* Enable UART2 transmitter and UART2 receiver */
    USART2->CR1 |= (USART_CR1_RE |  USART_CR1_TE);             // Enable transmitter
    USART2->CR1 |= USART_CR1_UE;             // Enable USART2
}

void dma1_stream5_init(void){
    /* Enable DMA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

    /* Disable DMA stream while configuring */
    DMA1_Stream5->CR &= ~DMA_SxCR_EN;
    while (DMA1_Stream5->CR & DMA_SxCR_EN) {} // Wait until DMA is fully disabled

    /* Clear DMA stream flags */
    DMA1->HIFCR |= DMA_HIFCR_CTCIF5 | DMA_HIFCR_CHTIF5 | DMA_HIFCR_CTEIF5 | DMA_HIFCR_CDMEIF5 | DMA_HIFCR_CFEIF5;

    /* Disable circular mode */
	DMA1_Stream5->CR |= DMA_SxCR_CIRC;

	/* DMA flow control */
	DMA1_Stream5->CR &= ~DMA_SxCR_PFCTRL;

	/* MBURST 01 INCR4 */
	DMA1_Stream5->CR |= DMA_SxCR_MBURST_0;

	/* PSIZE BYTE 00 */
	DMA1_Stream5->CR &= ~(DMA_SxCR_PSIZE_0 | DMA_SxCR_PSIZE_1);

	/* MSIZE HALFWORD 01 */
	DMA1_Stream5->CR |= DMA_SxCR_MSIZE_0;

	/* FIFO threshold */
	DMA1_Stream5->FCR |= DMA_SxFCR_FTH_0; // Full FIFO

	/* Enable FIFO interrupt enable */
	DMA1_Stream5->FCR |= DMA_SxFCR_FEIE;

	/* Configure destination as USART2 */
	USART2->CR3 |= USART_CR3_DMAR;
}

 void dma1_stream6_init(void) {
    /* Enable DMA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

    /* Disable DMA stream while configuring */
    DMA1_Stream6->CR &= ~DMA_SxCR_EN;
    while (DMA1_Stream6->CR & DMA_SxCR_EN) {} // Wait until DMA is fully disabled

    /* Clear DMA stream flags */
    DMA1->HIFCR |= DMA_HIFCR_CTCIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CFEIF6;

    /* Disable circular mode */
    DMA1_Stream6->CR &=~ DMA_SxCR_CIRC;

    /* DMA flow control */
    DMA1_Stream6->CR &= ~DMA_SxCR_PFCTRL;

    /* MBURST 01 INCR4 */
    DMA1_Stream6->CR |= DMA_SxCR_MBURST_0;

    /* PSIZE BYTE 00 */
    DMA1_Stream6->CR &= ~(DMA_SxCR_PSIZE_0 | DMA_SxCR_PSIZE_1);

    /* MSIZE HALFWORD 01 */
    DMA1_Stream6->CR |= DMA_SxCR_MSIZE_0;

    /* FIFO threshold */
    DMA1_Stream6->FCR |= DMA_SxFCR_FTH_0; // Full FIFO

    /* Enable FIFO interrupt enable */
    DMA1_Stream6->FCR |= DMA_SxFCR_FEIE;

    /* Configure destination as USART2 */
    USART2->CR3 |= USART_CR3_DMAT;
}

 void dma1_start_stream5(uint32_t src, uint32_t rx_dma_buffer, uint32_t buffer_size) {
     // Configure DMA stream parameters
     DMA1_Stream5->PAR = src;            // Source address
     DMA1_Stream5->M0AR = rx_dma_buffer; // Destination buffer
     DMA1_Stream5->NDTR = buffer_size;           // Number of data to transfer
     DMA1_Stream5->CR |= DMA_SxCR_CHSEL_2;   // Channel 4 selected
     DMA1_Stream5->CR |= DMA_SxCR_MINC; // Increment memory address after each transfer
     DMA1_Stream5->CR &=~ DMA_SxCR_DIR; // Transfer data from peripheral to memory
     DMA1_Stream5->CR |= DMA_SxCR_HTIE; // Enable transfer complete interrupt
     DMA1_Stream5->CR |= DMA_SxCR_TCIE; // Enable transfer complete interrupt

     // Disable FIFO mode
     DMA1_Stream5->FCR = 0;

     NVIC_EnableIRQ(DMA1_Stream5_IRQn);

     // Enable DMA stream
     DMA1_Stream5->CR |= DMA_SxCR_EN;
 }

void dma1_start_stream6(uint32_t src, uint32_t dst, uint32_t len) {
    // Configure DMA stream parameters
    DMA1_Stream6->PAR = dst;            // Destination address
    DMA1_Stream6->M0AR = src;           // Source address
    DMA1_Stream6->NDTR = len;           // Number of data to transfer
    DMA1_Stream6->CR |= DMA_SxCR_CHSEL_2;   // Channel 4 selected
    DMA1_Stream6->CR |= DMA_SxCR_MINC; // Increment memory address after each transfer
    DMA1_Stream6->CR |= DMA_SxCR_DIR_0; // Transfer data from memory to peripheral
    DMA1_Stream6->CR |= DMA_SxCR_TCIE; // Enable transfer complete interrupt

    // Disable FIFO mode
    DMA1_Stream6->FCR = 0;

    NVIC_EnableIRQ(DMA1_Stream6_IRQn);

    // Enable DMA stream
    DMA1_Stream6->CR |= DMA_SxCR_EN;
}

void DMA1_Stream5_IRQHandler(void)
{
	/* Check Transfer error flag */
	if (DMA1->HISR & DMA_HISR_TEIF5)
	{
        DMA1->HIFCR |= DMA_HIFCR_CTEIF5;
	}

	/* Check FIFO error flag */
	if (DMA1->HISR & DMA_HISR_FEIF5)
	{

	}

	/* Check Half transfer flag */
	if (DMA1->HISR & DMA_HISR_HTIF5)
	{
        DMA1->HIFCR |= DMA_HIFCR_CHTIF5;

//        uint32_t length = ((RX_DMA_BUFFER_SIZE / 2) - dma_head + RX_DMA_BUFFER_SIZE) % RX_DMA_BUFFER_SIZE;
        uint32_t length = (RX_DMA_BUFFER_SIZE - DMA1_Stream5->NDTR) - dma_head;

        // Update the head value of dma_buffer by the just read bytes
        update_crc_buffer_pointer(&dma_head, RX_DMA_BUFFER_SIZE, length);

        copyBuffer(rx_dma_buffer, rx_data_buffer, dma_head, dma_tail, data_head, data_tail);

        // Update the tail value of dma_buffer by the length
        update_crc_buffer_pointer(&dma_tail, RX_DMA_BUFFER_SIZE, length);

        // Update the head value of data_buffer by the just read bytes
        update_crc_buffer_pointer(&data_head, RX_DATA_BUFFER_SIZE, length);

        GPIOA->ODR ^= GPIO_ODR_ODR_5;
	}

	/* Check full transfer flag */
    if (DMA1->HISR & DMA_HISR_TCIF5)
    {
    	DMA1->HIFCR |= DMA_HIFCR_CTCIF5;

    	uint32_t length = RX_DMA_BUFFER_SIZE - dma_head + (RX_DMA_BUFFER_SIZE - DMA1_Stream5->NDTR);

		// Update the head value of dma_buffer by the just read bytes
		update_crc_buffer_pointer(&dma_head, RX_DMA_BUFFER_SIZE, length);

		copyBuffer(rx_dma_buffer, rx_data_buffer, dma_head, dma_tail, data_head, data_tail);

        // Update the tail value of dma_buffer by the length
        update_crc_buffer_pointer(&dma_tail, RX_DMA_BUFFER_SIZE, length);

        // Update the head value of data_buffer by the just read bytes
        update_crc_buffer_pointer(&data_head, RX_DATA_BUFFER_SIZE, length);

    	GPIOA->ODR ^= GPIO_ODR_ODR_5;

        dma1_start_stream6((uint32_t)(rx_data_buffer), (uint32_t)(&USART2->DR), sizeof(rx_data_buffer) - 1);
    }
}

void DMA1_Stream6_IRQHandler(void)
{
	/* Check Transfer error flag */
	if (DMA1->HISR & DMA_HISR_TEIF6)
	{
        DMA1->HIFCR |= DMA_HIFCR_CTEIF6;
	}

	/* Check FIFO error flag */
	if (DMA1->HISR & DMA_HISR_FEIF6)
	{
        DMA1->HIFCR |= DMA_HIFCR_CFEIF6;

	}

	/* Check Half transfer flag */
	if (DMA1->HISR & DMA_HISR_HTIF6)
	{
        DMA1->HIFCR |= DMA_HIFCR_CHTIF6;
        GPIOA->ODR ^= GPIO_ODR_ODR_5;
	}

	/* Check full transfer flag */
    if (DMA1->HISR & DMA_HISR_TCIF6)
    {
    	GPIOA->ODR ^= GPIO_ODR_ODR_5;
        DMA1->HIFCR |= DMA_HIFCR_CTCIF6;
    }
}

void copyBuffer(char *src, char *dst, uint32_t src_head, uint32_t src_tail, uint32_t dst_head, uint32_t dst_tail) {
    // Iterate through each element in the source buffer
    do {
        // Copy element from source buffer to destination buffer
        dst[dst_head++] = src[src_tail++];

        // Adjust source index for wraparound
        if (src_tail > RX_DMA_BUFFER_SIZE)
            src_tail = 0;

        // Adjust destination index for wraparound
        if (dst_head > RX_DATA_BUFFER_SIZE)
            dst_head = 0;

    } while (src_tail != src_head);
}

// Increment  pointer, wrapping around if it exceeds buffer size
void update_crc_buffer_pointer(uint32_t *pointer, uint32_t buffer_size, uint32_t increment) {
    *pointer = (*pointer + increment) % buffer_size;
}

