#include "stm32f4xx.h"

void gpioa_led_init(void)
{
    /* Enable clock for GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Set pin 5 of GPIOA as output */
    GPIOA->MODER |= GPIO_MODER_MODER5_0;
    GPIOA->MODER &= ~GPIO_MODER_MODER5_1;
}
