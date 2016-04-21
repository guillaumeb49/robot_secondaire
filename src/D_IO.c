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
	// Initialize the pin connected to the ULN2003 (PA8)
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	GPIOA->MODER &= ~(0x03 << 16);
	GPIOA->MODER |= 0x01 << 16;	// PA8 as output
	GPIOA->ODR &= ~(0x01 << 8);	// low state



	// Initialize the LEDs (PORT B)
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	GPIOB->MODER |= 1;		// Output for pin PB0
	GPIOB->MODER |= 4;		// Output for pin PB1
	GPIOB->MODER |= 0x10;	// Output for pin PB2

	// Initialize the pins connected to the L298N
	GPIOB->MODER |= (1 << 20);	// Output for pin PB10
	GPIOB->MODER |= (1 << 22);	// Output for pin PB11
	GPIOB->MODER |= (1 << 24);	// Output for pin PB12

	// Initialize ChA_gauche  (connected to PB4)
	GPIOB->MODER &=~(0x03 << 8);	// Input for pin PB4
	GPIOB->MODER &=~(0x03 << 10);	// Input for pin PB5




}
