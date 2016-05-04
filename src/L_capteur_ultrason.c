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
	GPIOB->MODER &= ~((0x3 << 12)|(0x3 << 14));

	// Initialiser les timers associes
	// Echo Avant 	: PB7 	TIM4 CH2 (ALT 2) 	Count only when PB7 is high
	// Echo Arriere : PB6	TIM4 CH1 (ALT 2)	Count only when PB6 is high

	// PB6
	GPIOB->MODER |= (0x2 << 12);
	GPIOB->AFR[0] &= ~(0xF << 24);
	GPIOB->AFR[0] |= (2 << 24);

	// PB7
	GPIOB->MODER |= (0x2 << 14);
	GPIOB->AFR[0] &= ~(0xF << 28);
	GPIOB->AFR[0] |= (2 << 28);

	// Configuration Timer
	TIM4->PSC = 72;	// 8MHz /8 => 1us

	// Channel 1
	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S;
	TIM4->CCMR1 |= 0x1;

	TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;
	TIM4->CCER |= (1 << 1 | 1 << 3);	// Both edges
	TIM4->CCMR1 &= ~(TIM_CCMR1_IC1PSC);

	TIM4->CCER |= TIM_CCER_CC1E;

	// Channel 2
	TIM4->CCMR1 &= ~TIM_CCMR1_CC2S;
	TIM4->CCMR1 |= 0x1 << 8;

	TIM4->CCMR1 &= ~TIM_CCMR1_IC2F;
	TIM4->CCER |= (1 << 7 | 1 << 5);	// Both edges
	TIM4->CCMR1 &= ~(TIM_CCMR1_IC2PSC);

	TIM4->CCER |= TIM_CCER_CC2E;

	TIM4->CR1 |= TIM_CR1_CEN;	// Enable counter

}


/**
 *
 */
int16_t F_generer_trig(E_TRIG trigger)
{
	uint16_t temps = 0;
	uint32_t longueur = 0;
	uint32_t t1 = 0;
	uint32_t t2 = 0;

	static uint8_t timer_timeout = 0;
	uint8_t timeout = 0;

	if(trigger == TRIG_DROIT)
	{
		GPIOB->ODR |= 1 << 4;
		for(temps=0; temps<80;temps++);
		GPIOB->ODR &= ~(1 << 4);

		TIM4->CNT = 0;
		TIM4->SR &= ~TIM_SR_CC1IF;

		// stocker valeur timer pour timeout
		timer_timeout = timer_10ms;
		// Attendre front montant ou timeout
		while((!(TIM4->SR & TIM_SR_CC1IF)) && (timeout == 0))
		{
			if((timer_10ms - timer_timeout) >= 4)
			{
				timeout = 1;
			}
		}

		// Si timeout
		if(timeout == 1)
		{
			return 0;	// retourner une erreur
		}

		t1 = TIM4->CCR1;
		TIM4->SR &= ~TIM_SR_CC1IF;
		// stocker valeur timer pour timeout
		timer_timeout = timer_10ms;
		// Attendre front descendant
		while((!(TIM4->SR & TIM_SR_CC1IF)) && (timeout == 0))
		{
			if((timer_10ms - timer_timeout) >= 4)
			{
				timeout = 1;
			}
		}

		TIM4->CNT = 0;
		TIM4->SR &= ~TIM_SR_CC1IF;

		// Si timeout
		if(timeout == 1)
		{
			return 0;	// retourner une erreur
		}
		t2 = TIM4->CCR1;
		longueur = (t2 - t1)/58;
	}
	else
	{
		GPIOB->ODR |= 1 << 5;
		for(temps=0; temps<80;temps++);
		GPIOB->ODR &= ~(1 << 5);
		TIM4->SR &= ~TIM_SR_CC2IF;
		// stocker valeur timer pour timeout
		timer_timeout = timer_10ms;
		// Attendre front montant
		while((!(TIM4->SR & TIM_SR_CC2IF)) && (timeout == 0))
		{
			if((timer_10ms - timer_timeout) >= 4)
			{
				timeout = 1;
			}
		}

		// Si timeout
		if(timeout == 1)
		{
			return 0;	// retourner une erreur
		}

		t1 = TIM4->CCR2;
		TIM4->SR &= ~TIM_SR_CC2IF;

		// Attendre front descendant
		while((!(TIM4->SR & TIM_SR_CC2IF)) && (timeout == 0))
		{
			if((timer_10ms - timer_timeout) >= 4)
			{
				timeout = 1;
			}
		}

		TIM4->CNT = 0;
		TIM4->SR &= ~TIM_SR_CC2IF;

		// Si timeout
		if(timeout == 1)
		{
			return 0;	// retourner une erreur
		}

		// Calcul de la longueur
		t2 = TIM4->CCR2;
		longueur = (t2 - t1)/58;
	}

	printf("Longueur %d: %d cm "
			"\r\n", (int)trigger,(int)(longueur));


	return (int16_t)longueur;
}
