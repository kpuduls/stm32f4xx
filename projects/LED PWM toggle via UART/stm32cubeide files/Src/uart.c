#include "uart.h"

#define GPIOAEN				(1U<<0)
#define UART1EN				(1U<<4)

#define SYSFREQ				16000000
#define APB2_CLK			SYSFREQ
#define UART_BAUDRATE		115200

#define CR1_TE				(1U<<3)
#define CR1_RE				(1U<<2)

#define CR1_UE				(1U<<13)

#define SR_TXE				(1U<<7)
#define SR_RXNE				(1U<<5)


static void uart_set_baudrate(USART_TypeDef *UARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);

void uart1_write(int ch);
char uart1_read(void);
int __io_putchar(int ch);


int __io_putchar(int ch)
{
	uart1_write(ch);
	return ch;
}


void uart1_rxtx_init(void)
{
	// enable gpioa clock
	RCC->AHB1ENR |= GPIOAEN;

	// set p9, p10 alternate
	//p9
	GPIOA->MODER &=~ (1U<<18);
	GPIOA->MODER |= (1U<<19);
	//p10
	GPIOA->MODER &=~ (1U<<20);
	GPIOA->MODER |= (1U<<21);
	//alternate function set
	//p9
	GPIOA->AFR[1] |= (1U<<4);
	GPIOA->AFR[1] |= (1U<<5);
	GPIOA->AFR[1] |= (1U<<6);
	GPIOA->AFR[1] &=~ (1U<<7);
	//p10
	GPIOA->AFR[1] |= (1U<<8);
	GPIOA->AFR[1] |= (1U<<9);
	GPIOA->AFR[1] |= (1U<<10);
	GPIOA->AFR[1] &=~ (1U<<11);

	RCC->APB2ENR |= UART1EN;

	uart_set_baudrate(USART1, APB2_CLK, UART_BAUDRATE);

	//enable rx tx directions
	USART1->CR1 = (CR1_TE | CR1_RE);
	USART1->CR1 |= CR1_UE;
}


char uart1_read(void)
{
	while(!(USART1->SR & SR_RXNE)){}
	return USART1->DR;
}


void uart1_write(int ch)
{
	while(!(USART1->SR & SR_TXE)){}
	USART1->DR = (ch & 0xFF);
}


static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}


static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	return (PeriphClk + (BaudRate/2U))/BaudRate;
}
