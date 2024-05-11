#include "stm32f4xx.h"
#include "tim.h"

#define TIM2EN				(1U<<0)
#define TIM3EN				(1U<<1)
#define CR1_CEN				(1U<<0)
#define OC_TOGGLE			((1U<<4) | (1U<<5))
#define OC_PWM				((1U<<5) | (1U<<6))
#define CCER_CC1E			(1U<<0)
#define GPIOAEN				(1U<<0)
#define AFR5_TIM			(1U<<20)
#define CCER_CC1P			(1U<<1)
#define CC1S				((1U<<0) | (1U<<1))
#define OC1PE				(1U<<3)
#define ARPE				(1U<<7)
#define CMS					((1U<<5) | (1U<<6))
#define DIR					(1U<<4)
#define AFR6_TIM			(1U<<25)
#define CCR1_CC1S			(1U<<0)


void tim2_1hz_init(void)
{
	//enable clock access to tim2
	RCC->APB1ENR |= TIM2EN;

	//set prescaler value
	TIM2->PSC = 16000 - 1;    		//1600 000 /1 600 = 10 000

	//set auto-reload value
	TIM2->ARR = 10000 - 1; 			//10 000 / 10 000 = 1 hz

	//clear timer counter
	TIM2->CNT = 0;

	//enable the counter
	TIM2->CR1 = CR1_CEN;
}


void tim2_pa5_pwm(void)
{
	//enable cliock access to gpioa
	RCC->AHB1ENR |= GPIOAEN;

	//set pa5 as alternate
	GPIOA->MODER &=~ (1U<<10);
	GPIOA->MODER |= (1U<<11);

	//set pa5 alternate type to tim2_ch1 AF01
	GPIOA->AFR[0] |= AFR5_TIM;

	//enable clock access to tim2
	RCC->APB1ENR |= TIM2EN;

	//set prescaler value
	TIM2->PSC = 160 - 1;    		//1600 000 /160 = 1000 10khz

	//set auto-reload value
	TIM2->ARR = 100 - 1;

	//set tim2 ch1 as output
	TIM2->CCMR1 &=~ CC1S;

	//set output compare pwm mode
	TIM2->CCMR1 |= OC_PWM;

	//set polarity for pwm
	TIM2->CCER &=~ CCER_CC1P;

	//set duty cycle CCR
	TIM2->CCR1 = 500;

	//set preload enable
	TIM2->CCMR1 |= OC1PE;

	//enable preload
	TIM2->CR1 |= ARPE;

	//set upcounting edge
	TIM2->CR1 &=~ CMS;

	//set direction
	TIM2->CR1 &=~ DIR;

	//enable tim2 ch1 in compare mode
	TIM2->CCER |= CCER_CC1E;

	//clear timer counter
	TIM2->CNT = 0;

	//enable the counter
	TIM2->CR1 |= CR1_CEN;
}

void tim3_input_capture(void)
{
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER |= (1U<<13);
	GPIOA->MODER &=~ (1U<<12);

	GPIOA->AFR[0] |= AFR6_TIM;

	RCC->APB1ENR |= TIM3EN;

	TIM3->PSC = 160 - 1;    		//1600 000 /160 = 1000 10khz

	TIM3->CCR1 = CCR1_CC1S;

	TIM3->CCER |= CCER_CC1E;

	TIM3->CR1 = CR1_CEN;

}
