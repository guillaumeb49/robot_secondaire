/**
 *	\author Guillaume B
 *
 *	\brief
 *
 *	\date 12/03/2016
 */


#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "D_IO.h"
#include "D_UART.h"
#include "D_system_clock.h"
#include "D_I2C.h"
#include "L_capteur_ultrason.h"

#include "common.h"

// Buffer de reception I2C
uint8_t buffer_i2c_rx[255];

// Buffer de transmission I2C
uint8_t buffer_i2c_tx[255];

// index buffer_rx
uint8_t index_buffer_rx;

// index buffer_tx
uint8_t index_buffer_tx;

// statut de la derni�re op�ration
E_status status_operation;

// Timer 100 us
uint8_t timer_100us;

// Timer 1 ms
uint8_t timer_1ms;

// Timer 10 ms
uint8_t timer_10ms;

// Timer 100 ms
uint8_t timer_100ms;

// Timer 1s
uint16_t timer_1s;


int main()
{
	volatile uint32_t i = 0;
	volatile uint32_t j = 0;
	uint8_t retour = 0;
	// Init IOs
	F_init_IO();

	F_init_clocks();
	// Init UART debug (2)
	F_init_UART_debug();

	F_init_I2C();

	//F_init_capteur_ultrasons();


	printf("Hello World\r\n");




	LED_ORANGE_ON();
	LED_GREEN_OFF();
	LED_RED_OFF();




	i = 0;

	// Attendre que la prise jack soit retiree


	while(1)
	{
		F_transmit_to_slave(0, 2);
		for(i=0;i<10;i++)
		{
			for(j=0;j<65000; j++);
		}

		LED_ORANGE_ON();
		F_soft_reset_I2C();
		for(i=0;i<10;i++)
		{
			for(j=0;j<65000; j++);
		}
		LED_ORANGE_OFF();
		for(i=0;i<10;i++)
				{
					for(j=0;j<65000; j++);
				}
		//F_generer_trig(TRIG_AVANT);


		// Traiter recption I2C
		// Si timeout
			// Lire buffer reception
			// Indiquer status pending
			status_operation = E_STATUS_PENDING;
			// retour = Executer F_ExecuteCommande(buffer_i2c_receive)
			if(retour == 0)
			{
				status_operation = E_STATUS_OK;
			}
			else
			{
				status_operation = E_STATUS_ERROR;
			}




			// Si temps de realiser la Funny Action
			if(timer_1s == 92)
			{
				// Ouvrir parasol
			}
	}

	return 0;
}




//


// Free running timer
void F_timer_TM2(void)
{

}


// ----------------------------------------------------------------------------
