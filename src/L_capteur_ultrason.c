/*
 * L_capteur_ultrason.c
 *
 *  Created on: 3 avr. 2016
 *      Author: guill
 */

#include "L_capteur_ultrason.h"


void F_init_capteur_ultrasons(void)
{
	// Initialiser les IO connectees aux capteurs ultrasons
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	// Enable Timer 4
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

	// Sorties Trig PB4 et PB5 : Open-drain No pull-up No pull down
	GPIOB->OTYPER 	|= (1 << 4) | (1 << 5);			// Open Drain
	GPIOB->OSPEEDR 	|= (0x3 << 8) | (0x3 << 10);	// High speed
	GPIOB->PUPDR	&= ~((0x3 << 8)|(0x3 << 10));	// No pull-up No pull-down
	GPIOB->MODER	&= ~((0x3 << 8)|(0x3 << 10));
	GPIOB->MODER	|= (1 << 8) | (1 << 10);		// Output

	// Entrees PB6 et PB7 : No pull-up No pull down
	GPIOB->PUPDR &= ~((0x3 << 12)|(0x3 << 14));		// No pull-up No pull down
	GPIOB->MODER &= ~((0x3 << 12)|(0x3 << 14));		// Input

	// Initialiser les timers associes
	// Echo Avant 	: PB7 	TIM4 CH2 (ALT 2) 	Count only when PB7 is high
	// Echo Arriere : PB6	TIM4 CH1 (ALT 2)	Count only when PB6 is high

	GPIOB->MODER &= ~(0x3 << 12);
	GPIOB->MODER |= (0x2 << 12);
	GPIOB->AFR[0] &= ~(0xF << 24);
	GPIOB->AFR[0] |= (2 << 24);


	//TIM4->CR2 |= TIM_CR2_TI1S;	// TIM4_CH1 and TIM4_CH2 pins connected to TI1 input (XOR combination)
/*	TIM4->CR2 &= ~(TIM_CR2_TI1S);


	TIM4->PSC = 1000;

	TIM4->CCMR1 &= ~(0x3 << 0);
	TIM4->CCMR1	|=  (0x1 << 0);	 //CC1S = 01

	TIM4->CCER |= TIM_CCER_CC1P;	// TIMx_CCER CC1P = 1 et CC1NP = 0							// Detect High level
	//TIM4->CCER &= ~TIM_CCER_CC1NP;
	TIM4->CCER |= TIM_CCER_CC1NP;

	TIM4->SMCR &= ~(TIM_SMCR_SMS);							// Gated mode SMS = 101 TS = 101
	TIM4->SMCR |= 5;
	TIM4->SMCR &= ~TIM_SMCR_TS;
	TIM4->SMCR |= (5 << 4);

*/

	/*TIM4->CCMR1 &= ~(0x3 << 0);
	TIM4->CCMR1 |= (0x1 << 0);
	TIM4->PSC = 1000;
	TIM4->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
	TIM4->CCER |= TIM_CCER_CC1P | TIM_CCER_CC1NP;	// Both rising and falling edges

	TIM4->CCMR1 &= ~(TIM_CCMR1_IC1PSC);


	TIM4->CCER |= TIM_CCER_CC1E;

	TIM4->CR1 |= TIM_CR1_CEN;	// Enable the Timer
*/


	TIM4->PSC = 72;	// 8MHz /8 => 1us

	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S;
	TIM4->CCMR1 |= 0x1;

	TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;
	TIM4->CCER |= (1 << 1 | 1 << 3);

	TIM4->CCMR1 &= ~(TIM_CCMR1_IC1PSC);

	TIM4->CCER |= TIM_CCER_CC1E;

	TIM4->CR1 |= TIM_CR1_CEN;

}


/**
 *
 */
void F_generer_trig(E_TRIG trigger)
{
	uint16_t temps = 0;
	uint32_t longueur = 0;
	uint32_t t1 = 0;
	uint32_t t2 = 0;

	if(trigger == TRIG_AVANT)
	{

	}
	else
	{

	}


	GPIOB->ODR |= 1 << 4;
	GPIOB->ODR |= 1 << 5;

	for(temps=0; temps<80;temps++);

	GPIOB->ODR &= ~(1 << 4);
	GPIOB->ODR &= ~(1 << 5);

	TIM4->CNT = 0;
	TIM4->SR &= ~TIM_SR_CC1IF;
	// Attendre front montant
	while(!(TIM4->SR & TIM_SR_CC1IF));
	t1 = TIM4->CCR1;
	TIM4->SR &= ~TIM_SR_CC1IF;


	// Attendre front descendant
	while(!(TIM4->SR & TIM_SR_CC1IF));
	t2 = TIM4->CCR1;
	longueur = t2 - t1;
	TIM4->CNT = 0;

	longueur = longueur /58;
	/*printf("Longueur : %d cm "
			"\r\n", (int)(longueur/58));
*/



}
