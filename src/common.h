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
#include "D_IO.h"

#define LENGTH_CMD 6

#define THETA_0		0
#define THETA_45	32
#define THETA_90	64
#define THETA_135	96
#define THETA_180	128
#define THETA_225	159
#define THETA_270	0
#define THETA_315	0
#define THETA_360	0

/**
 * Statut de la derniere operation executee
 */
typedef enum{

	E_STATUS_PENDING = 0,
	E_STATUS_OK = 1,
	E_STATUS_ERROR = 2
}E_status;

/*
 * Definition d'un point
 */
typedef struct{
	uint16_t x;
	uint16_t y;
	uint8_t theta;
}S_point;

typedef enum{
	CMD_GOTO = 1,
	CMD_RECULER = 2,
	CMD_START_STOP = 3
}E_CMD;


/*
 * Definition d'un point
 */
typedef struct{

	E_CMD commande;
	uint16_t param1;
	uint16_t param2;
	uint8_t param3;
}S_commande;

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

// Flag recuperer valeur capteur ultrason
extern uint8_t recuperer_ultrason;

// Flag nouvelle reception I2C
extern uint8_t nb_data_i2c;

// Flag timeout I2C
extern uint8_t timeout_i2c;

#endif /* COMMON_H_ */
