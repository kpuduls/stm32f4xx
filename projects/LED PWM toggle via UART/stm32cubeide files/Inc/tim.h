#ifndef TIM_H_
#define TIM_H_

#define SR_UIF				(1U<<0)
#define SR_CC1IF			(1U<<1)

void tim2_pa5_pwm(void);
void tim2_1hz_init(void);
void tim3_input_capture(void);

#endif /* TIM_H_ */
