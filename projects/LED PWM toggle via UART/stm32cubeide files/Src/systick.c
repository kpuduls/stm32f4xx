#include "stm32f4xx.h"
#include "systick.h"

#define SYSTICK_LOAD_VAL		16000		//1 ms
#define CTRL_ENABLE					(1U<<0)
#define CTRL_CLKSRC				(1U<<2)		//processor clock
#define CTRL_COUNTFLAG			(1U<<16)


void systickDelayMs(int delay)
{
	//configure systick
	//reload with number of clocks per ms
	SysTick->LOAD = SYSTICK_LOAD_VAL;

	//clear systick current val
	SysTick->VAL = 0;

	//enable systick and select internal clk src
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	for(int i=0; i<delay; i++)
	{
		//wait until count flag set
		while((SysTick->CTRL & CTRL_COUNTFLAG) == 0){}
	}

	//disable
	SysTick->CTRL = 0;
}
