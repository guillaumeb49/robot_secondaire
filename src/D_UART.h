/*
 * D_UART.h
 *
 *  Created on: 12 mars 2016
 *      Author: guill
 */

#ifndef D_UART_H_
#define D_UART_H_

#include "stm32f30x.h"

/** Init the USART 2 for debugging */
void F_init_UART_debug();

/** Send one char over the USART2 */
void F_usart2_put(uint8_t ch);

/** Receive one char from the USART2 */
uint8_t F_usart2_get(void);

#endif /* D_UART_H_ */
