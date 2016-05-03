/*
 * A_servo_moteur.h
 *
 *  Created on: 24 avr. 2016
 *      Author: guill
 */

#ifndef A_SERVO_MOTEUR_H_
#define A_SERVO_MOTEUR_H_

#include "D_Timer.h"



typedef enum{
	SERVO_0deg,
	SERVO_90deg,
	SERVO_120deg,
	SERVO_180deg

}E_SERVO_POSITION;


void F_move_servo1(E_SERVO_POSITION pos);
void F_move_servo2(E_SERVO_POSITION pos);
void F_move_servo3(E_SERVO_POSITION pos);

#endif /* A_SERVO_MOTEUR_H_ */
