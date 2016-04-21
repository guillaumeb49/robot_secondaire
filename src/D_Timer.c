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
	// Init Timer 2

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	// Enable clock on Timer 2

	// Autoreload enable
	TIM2->CR1 |= TIM_CR1_ARPE;
	TIM2->CR1 &= ~TIM_CR1_CMS;	// Counts up or down depending on the direction bit
	TIM2->CR1 &= ~TIM_CR1_DIR;	// Upcounting

	// Auto-reload value
	TIM2->ARR = 7200;	// 100 us

	// Enable Interrupt UIE (overflow / underflow)
	TIM2->DIER |= TIM_DIER_UIE;

	// Enable TIM2 interrupt
	NVIC_EnableIRQ(TIM2_IRQn);

	// Enable Timer 1
	TIM2->CR1 |= TIM_CR1_CEN;

}
