#include "stm32f4xx.h"
#include "stdint.h"
#include "stdio.h"
#include "uart.h"
#include "tim.h"

#define PIN13				(1U<<13)
#define LED_PIN				PIN13
#define GPIOCEN				(1U<<2)

char val;


int main(void)
{
	uart1_rxtx_init();
	tim2_pa5_pwm();

	while(1)
	{
		val = uart1_read();
		//set duty cycle CCR
		TIM2->CCR1 = (int)val;
	}
}
