/*
 * L_capteur_ultrason.h
 *
 *  Created on: 5 avr. 2016
 *      Author: guill
 */

#ifndef L_CAPTEUR_ULTRASON_H_
#define L_CAPTEUR_ULTRASON_H_

#include "stm32f30x.h"
#include <stdio.h>

#include "common.h"


typedef enum
{
	TRIG_DROIT,
	TRIG_GAUCHE,
}E_TRIG;





void F_init_capteur_ultrasons(void);
uint16_t F_generer_trig(E_TRIG trigger);

#endif /* L_CAPTEUR_ULTRASON_H_ */
