/*
 * A_servo_moteur.c
 *
 *  Created on: 24 avr. 2016
 *      Author: guill
 */


#include "A_servo_moteur.h"



void F_move_servo1(E_SERVO_POSITION pos)
{
	switch(pos)
	{
	case SERVO_0deg:
		break;
	case SERVO_90deg:
		break;
	case SERVO_180deg:
		break;
	}
}

void F_move_servo2(E_SERVO_POSITION pos)
{
	switch(pos)
		{
		case SERVO_0deg:
			break;
		case SERVO_90deg:
			break;
		case SERVO_180deg:
			break;
		}
}

void F_move_servo3(E_SERVO_POSITION pos)
{
	switch(pos)
		{
		case SERVO_0deg:
			TIM17->CCR1 = 40;
			break;

		case SERVO_90deg:
			TIM17->CCR1 = 110;
			break;

		case SERVO_180deg:
			TIM17->CCR1 = 180;
			break;
		}
}
