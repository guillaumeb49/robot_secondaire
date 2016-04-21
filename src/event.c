/*
 * event.c
 *
 *  Created on: 13 avr. 2016
 *      Author: guill
 */

#include "stm32f30x.h"

#include "common.h"

// Interruptions

/**
 *
 */
void I2C2_EV_IRQHandler(void)
{
	// Si adresse match
	if(I2C2->ISR & I2C_ISR_ADDR)
	{

	}
	// Si buffer de reception peut etre lu
	else if(I2C2->ISR & I2C_ISR_RXNE)
	{
		// Lire buffer
	}
}

/**
 * @brief TIM2 : Timer 100us / 1 ms / 10 ms / 100 ms / 1s
 */
void TIM2_IRQHandler(void)
{
	// Si overflow
	if(TIM2->SR & TIM_SR_UIF)
	{
		timer_100us++;
		// Incrementer timer 100 us
		if(timer_100us == 10)
		{
			timer_100us = 0;
			timer_1ms++;
		}

		// Incrementer timer 1 ms
		if(timer_1ms == 10)
		{
			timer_1ms = 0;
			timer_10ms++;
		}

		// Incrementer timer 10 ms
		if(timer_10ms == 10)
		{
			timer_10ms = 0;
			timer_100ms++;
		}

		if(timer_100ms == 10)
		{
			timer_100ms = 0;
			timer_1s++;
		}
	}

}

