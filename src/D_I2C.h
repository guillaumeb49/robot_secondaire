/*
 * D_I2C.h
 *
 *  Created on: 30 mars 2016
 *      Author: guill
 */

#ifndef D_I2C_H_
#define D_I2C_H_

#include "stm32f30x.h"

// MASTER : 0
// SLAVE  : 1
#define I2C_MODE 0

#define SLAVE_ADDRESS 4


void F_init_I2C(void);
void F_soft_reset_I2C(void);
void F_transmit_to_slave(uint8_t rw_direction, uint8_t nb_data);


#endif /* D_I2C_H_ */
