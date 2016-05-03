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
		// Lever flag nouvelles donnees (donc demarrage du timeout)
		nb_data_i2c  = 0;


		//Si lecture

		// Traiter lecture
	}
	// Si buffer de reception peut etre lu
	else if(I2C2->ISR & I2C_ISR_RXNE)
	{
		nb_data_i2c++;
		// Lire buffer
	}
}

/**
 * @brief TIM2 : Timer  1 ms / 10 ms / 100 ms / 1s
 */
void TIM2_IRQHandler(void)
{
	// Si overflow
	if(TIM2->SR & TIM_SR_UIF)
	{
		TIM2->SR &= ~TIM_SR_UIF;
		timer_1ms++;

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

		// Compter toutes les 200 ms pour activer capteur ultrason
		if((timer_100ms % 2) != 0)
		{
			recuperer_ultrason = 1;
		}

		if(timer_100ms == 10)
		{
			timer_100ms = 0;
			timer_1s++;
			// Faire clignoter LED Orange toutes les secondes
			LED_ORANGE_TOGGLE();
		}
	}
}

