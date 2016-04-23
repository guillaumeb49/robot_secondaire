/*
 * D_I2C.c
 *
 *  Created on: 30 mars 2016
 *      Author: guill
 */

#include "D_I2C.h"

void F_init_I2C(void)
{
	// Enable clock on PORTA
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Enable clock for I2C2
	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

	// Configure PA9 in alternative mode
	GPIOA->MODER &= ~(0x03 << 18);
	GPIOA->MODER |= 0x02 << 18;

	GPIOA->AFR[1]&= ~(0x0F << 4);
	GPIOA->AFR[1]|= (4 << 4);	// AF4 : I2C

	GPIOA->OTYPER |= 0x01 << 9;


	// Configure PA10 in alternative mode
	GPIOA->MODER &= ~(0x03 << 20);
	GPIOA->MODER |= 0x02 << 20;

	GPIOA->AFR[1]&= ~(0x0F << 8);
	GPIOA->AFR[1]|= (4 << 8);	// AF4 : I2C

	GPIOA->OTYPER |= 0x01 << 10;

#if(I2C_MODE == 0)
	// Configure the I2C to work as a master device

	// Configure filters

	// In master mode, configure the SCL clock timing (400 kHz)
	I2C2->TIMINGR &= ~(0xF << 28);	// PRESC = 0
	I2C2->TIMINGR &= ~(0xF << 20);
	I2C2->TIMINGR |=  (0x3 << 20);	// SCLDEL = 3
	I2C2->TIMINGR &= ~(0xF << 16);
	I2C2->TIMINGR |= (1 << 16);		// SDADEL  = 1
	I2C2->TIMINGR &= ~(0xFF << 8);
	I2C2->TIMINGR |=  (0x3 << 8);	// SCLH = 3
	I2C2->TIMINGR &= ~(0xFF << 0);
	I2C2->TIMINGR |=  (0x9 << 0);	// SCLL = 9

	// Enable I2C2
	I2C2->CR1 |= I2C_CR1_PE;

#else
	// Configure the I2C to work as slave

	// Configure filters

	// Configure NOSTRETCH

	// Configure slave address

	// Enable Slave address
	I2C2->OAR1 &= ~(I2C_OAR1_OA1EN | 0x7FF);
	I2C2->OAR1 |= (SLAVE_ADDRESS << 1) & 0x3FF;
	I2C2->OAR1 |= I2C_OAR1_OA1EN;

	// Enable I2C2
	I2C2->CR1 |= I2C_CR1_PE;

#endif


	// Enable I2C2 interrupt
	NVIC_EnableIRQ(I2C2_EV_IRQn);

}


void F_soft_reset_I2C(void)
{
	uint32_t i = 0;

	I2C2->CR1 &= ~I2C_CR1_PE;
	for(i=0;i<500; i++);
	I2C2->CR1 |= I2C_CR1_PE;
}



/**
 * @brief
 *
 * Master Mode
 */
void F_transmit_to_slave(uint8_t rw_direction, uint8_t nb_data)
{
	volatile uint32_t test = 0;
	I2C2->CR2 |= I2C_CR2_AUTOEND;	// Automatic STOP mode (Send a STOP when all bytes have been transmitted)

	I2C2->CR2 &= ~I2C_CR2_ADD10;
	I2C2->CR2 &= ~I2C_CR2_SADD;
	I2C2->CR2 |= (SLAVE_ADDRESS << 1);	// Store the slave address

	test = I2C2->CR2;
	if(rw_direction)
	{
		I2C2->CR2 |= I2C_CR2_RD_WRN;
	}
	else
	{
		I2C2->CR2 &= ~I2C_CR2_RD_WRN;
	}

	I2C2->CR2 &= ~I2C_CR2_NBYTES;
	I2C2->CR2 |= (nb_data << 16);

	test = I2C2->CR2;

	// Send start
	I2C2->CR2 |= I2C_CR2_START;

	// Wait for ACK from the slave (ACK after START + ADDRESS received)
	while(!(I2C2->ISR & I2C_ISR_TXIS));

	// Write all the bytes to send to TXDR
	I2C2->TXDR = 0x55;
}

