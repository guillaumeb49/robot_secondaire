/*
 * D_system_clock.c
 *
 *  Created on: 14 mars 2016
 *      Author: Guillaume B
 */

#include "D_system_clock.h"
#include "D_IO.h"

void F_init_clocks()
{
	volatile uint32_t i = 0;
	// Configure Sysclock to use HSI
	RCC->CFGR &= ~(3 << 0);

	// Start HSE clock
	RCC->CR |= 1 << 16;	// HSE ON

	// Wait until HSE is ready
	while(!(RCC->CR & RCC_CR_HSERDY));

	// Disable PLL
	RCC->CR &= ~(1 << 24);	// PLL Off


	// Wait until the PLL is stopped
	while(RCC->CR & RCC_CR_PLLRDY);


	// Configure PLL (Max Frequency : 72 MHz (8MHz * 9))
	RCC->CFGR &= ~(RCC_CFGR_PLLMULL); // PLL x9
	RCC->CFGR |= 7 << 18; // PLL x9

	// Configure PLL entry Clock
	RCC->CFGR |= 1 << 16;

	// Enable PLL
	RCC->CR |= RCC_CR_PLLON;
	//Wait until the PLL is ready
	while(!(RCC->CR & RCC_CR_PLLRDY));

	FLASH->ACR |= FLASH_ACR_LATENCY_1; // one wait state
	FLASH->ACR |= FLASH_ACR_PRFTBE;  // prefetch enable

	// Configure Sysclock to use PLL output
	RCC->CFGR &= ~(RCC_CFGR_SW);	// PLL output
	RCC->CFGR |= (2 << 0);	// PLL output
	LED_GREEN_ON();
	// Wait until PLL is not used
	while(((RCC->CFGR & RCC_CFGR_SWS_0)) && (!(RCC->CFGR & RCC_CFGR_SWS_1)));
	i = RCC->CFGR;
	LED_GREEN_OFF();
}
