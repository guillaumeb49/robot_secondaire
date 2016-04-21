/*
 * D_UART.c
 *
 *  Created on: 12 mars 2016
 *      Author: Guillaume
 */


#include "D_UART.h"

/**
 * @brief Init the USART 2 for debugging purposes
 */
void F_init_UART_debug()
{
	// Select HSI as USART2 input clock (8MHz)
	RCC->CFGR3 |= 0x03 << 16;
	// Init clock USART 2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;


	// Init clock GPIO Port A
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure GPIOA[2:3] as alternate function mode
	GPIOA->MODER &= ~(0x3 << 4);
	GPIOA->MODER |= 0x02 << 4;
	GPIOA->MODER &= ~(0x3 << 6);
	GPIOA->MODER |= 0x02 << 6;

	GPIOA->AFR[0] &= ~(0x0F << 8);
	GPIOA->AFR[0] |= (7 << 8);
	GPIOA->AFR[0] &= ~(0x0F << 12);
	GPIOA->AFR[0] |= (7 << 12);


	// SWAP TX/RX pins if necessary
	//USART2->CR2 |= USART_CR2_SWAP;

	USART2->BRR = 417;


	// Enable Transmitter, Receiver, USART2 module
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

/**
 *	@brief Send a single character over UART 2
 */
void F_usart2_put(uint8_t ch)
{
	USART2->TDR = ch;

	//Loop until the end of transmission
	while(!(USART2->ISR & USART_ISR_TC));
	// Reset flag TC :Transmission complete
	USART2->ICR |= USART_ICR_TCCF;
}

/**
 *	@brief Get a single character from UART 2
 */
uint8_t F_usart2_get(void)
{
     while (!(USART2->ISR & USART_ISR_RXNE));
     // Read to reset RXNE flag
     return (uint8_t)USART2->RDR;
}
