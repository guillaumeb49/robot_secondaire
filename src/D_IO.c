/*
 * D_IO.c
 *
 *  Created on: 12 mars 2016
 *      Author: guill
 */

#include "D_IO.h"

/**
 * \brief Initialize the IO
 *
 */
void F_init_IO(void)
{
	// Initialize the LEDs (PORT B)
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	GPIOB->MODER |= 1;		// Output for pin PB0
	GPIOB->MODER |= 4;		// Output for pin PB1
	GPIOB->MODER |= 0x10;	// Output for pin PB2

		// High state pin PB0
}
