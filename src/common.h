/*
 * common.h
 *
 *  Created on: 19 avr. 2016
 *      Author: guill
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include "stm32f30x.h"
#include "event.h"


/**
 * Statut de la derniere operation executee
 */
typedef enum{

	E_STATUS_PENDING = 0,
	E_STATUS_OK = 1,
	E_STATUS_ERROR = 2
}E_status;


// Timer 100 us
extern uint8_t timer_100us;

// Timer 1 ms
extern uint8_t timer_1ms;

// Timer 10 ms
extern uint8_t timer_10ms;

// Timer 100 ms
extern uint8_t timer_100ms;

// Timer 1s
extern uint16_t timer_1s;


#endif /* COMMON_H_ */
