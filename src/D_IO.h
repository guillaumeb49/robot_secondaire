/*
 * D_IO.h
 *
 *  Created on: 12 mars 2016
 *      Author: guill
 */

#ifndef D_IO_H_
#define D_IO_H_

#include "stm32f30x.h"

void F_init_IO(void);

#define LED_GREEN_ON()		GPIOB->ODR |= 4;
#define LED_GREEN_OFF()		GPIOB->ODR &= ~4;
#define LED_GREEN_TOGGLE()	GPIOB->ODR = 4;

#define LED_ORANGE_ON()		GPIOB->ODR |= 2;
#define LED_ORANGE_OFF()	GPIOB->ODR &= ~2;
#define LED_ORANGE_TOGGLE()	GPIOB->ODR ^= 2;

#define LED_RED_ON()		GPIOB->ODR |= 1;
#define LED_RED_OFF()		GPIOB->ODR &= ~1;
#define LED_RED_TOGGLE()	GPIOB->ODR ^= 1;


#endif /* D_IO_H_ */
