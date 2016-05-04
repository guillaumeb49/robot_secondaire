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

#define SLAVE_ADDRESS 0x0F


void F_init_I2C(void);
void F_soft_reset_I2C(void);
uint8_t F_transmit_to_slave(uint8_t nb_data, uint8_t *data);
uint8_t F_receive_from_slave(uint8_t nb_data, uint8_t *data);

#endif /* D_I2C_H_ */
