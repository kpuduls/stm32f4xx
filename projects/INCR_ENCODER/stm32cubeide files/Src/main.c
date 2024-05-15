#include "stm32f4xx.h"

int main(void) {
	// Enable clock for GPIOB and TIM4
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

	// Configure GPIOB pins for alternate function mode
	GPIOB->MODER |= (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);
	GPIOB->AFR[0] |= (2 << (6 * 4)) | (2 << (7 * 4)); // AF2 for TIM4_CH3 and TIM4_CH4

	// Set ARR value
	TIM4->ARR = 2047;

	// Configure TIM4 for encoder mode on CH3 and CH4
	TIM4->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1; // Encoder mode 3

	// Enable TIM4
	TIM4->CR1 |= TIM_CR1_CEN;


    // Main loop
    while (1) {
        // Your application code here
    	uint32_t current_pos = TIM4->CNT;
    }

    return 0;
}
