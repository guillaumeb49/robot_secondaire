/*
 * D_Timer.c
 *
 *  Created on: 20 avr. 2016
 *      Author: guill
 */


#include "D_Timer.h"


/**
 *
 */
void F_Init_Timer(void)
{
	/* Init Timer 2 */

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	// Enable clock on Timer 2

	// Autoreload enable
	//TIM2->CR1 |= TIM_CR1_ARPE;
	TIM2->CR1 &= ~TIM_CR1_CMS;	// Counts up or down depending on the direction bit
	TIM2->CR1 &= ~TIM_CR1_DIR;	// Upcounting
	TIM2->CR1 &= ~TIM_CR1_UDIS;

	// Auto-reload value
	TIM2->ARR = 72000;	// 1 ms

	// Enable Interrupt UIE (overflow / underflow)
	TIM2->DIER |= TIM_DIER_UIE;

	// Enable TIM2 interrupt
	NVIC_EnableIRQ(TIM2_IRQn);

	// Enable Timer 2
	TIM2->CR1 |= TIM_CR1_CEN;

	/*************************/
	/* Init Timer 17 : PWM Servo 3 */
	RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;

	// 20 ms
	TIM17->ARR = 1440;	// auto reload value => 20 ms
	TIM17->PSC = 1000;	// prescaler 1000

	TIM17->CCR1 = 72; 	// 1ms

	TIM17->CCMR1 &=	~TIM_CCMR1_OC1M;	// PWM mode 1 Channel active as long as TIM17->CNT < TIM17->CCR1
	TIM17->CCMR1 |= (0x06 << 4);

	TIM17->CCMR1 |= TIM_CCMR1_OC1PE;

	TIM17->CCMR1 &= ~TIM_CCMR1_CC1S;	// Output

	TIM17->CR1 |= TIM_CR1_ARPE;

	TIM17->EGR |= TIM_EGR_UG;

	GPIOA->MODER &= ~ GPIO_MODER_MODER7;
	GPIOA->MODER |= 0x02 << 14;
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL7;
	GPIOA->AFR[0] |= 1 << 28;

	TIM17->CR1 |= TIM_CR1_CEN;
}
