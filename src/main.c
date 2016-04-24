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

#include "A_servo_moteur.h"

#include "D_Timer.h"

#include "common.h"

// Buffer de reception I2C
uint8_t buffer_i2c_rx[255];

// Buffer de transmission I2C
uint8_t buffer_i2c_tx[255];

// index buffer_rx
uint8_t index_buffer_rx;

// index buffer_tx
uint8_t index_buffer_tx;

// Flag nouvelle reception I2C
uint8_t nb_data_i2c;

// Flag timeout I2C
uint8_t timeout_i2c;

// statut de la dernière opération
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



/* Liste des points */
S_point liste_points[] = {{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0}
};



// Executer la commande contenu dans le buffer
void F_ExecuteCommande(uint8_t *buffer, uint8_t nb_data);






int main()
{
	volatile uint32_t i = 0;
	volatile uint32_t j = 0;
	uint8_t retour = 0;
	uint8_t nb_data_timeout_i2c = 0;
	uint8_t timer_10ms_timeout = 0;

	volatile uint16_t test = 0;

	E_SERVO_POSITION pos = SERVO_0deg;


	// Init IOs
	F_init_IO();

	F_init_clocks();
	// Init UART debug (2)
	F_init_UART_debug();

	//F_init_I2C();

	//F_init_capteur_ultrasons();
	F_Init_Timer();

	printf("Hello World\r\n");




	LED_ORANGE_ON();
	LED_GREEN_OFF();
	LED_RED_OFF();




	i = 0;

	// Attendre que la prise jack soit retiree


	while(1)
	{

		i = TIM17->CNT;
		j = TIM17->CCR1;
//		F_transmit_to_slave(0, 2);
//		for(i=0;i<10;i++)
//		{
//			for(j=0;j<65000; j++);
//		}
//
//		LED_ORANGE_ON();
//		F_soft_reset_I2C();
//		for(i=0;i<10;i++)
//		{
//			for(j=0;j<65000; j++);
//		}
//		LED_ORANGE_OFF();
//		for(i=0;i<10;i++)
//		{
//			for(j=0;j<65000; j++);
//		}
//		//F_generer_trig(TRIG_AVANT);
//
//		// Traiter reception I2C
//		/*if(nb_data_timeout_i2c == nb_data_i2c)
//		{
//			timer_10ms_timeout = timer_10ms;
//		}
//
//		if((timer_10ms-timer_10ms_timeout > 0) && (nb_data_i2c != 0))
//		{
//			// Timeout sur l'I2C
//			// Traiter la trame recu
//
//			// Lire buffer reception
//			// Indiquer status pending
//			status_operation = E_STATUS_PENDING;
//
//			//retour = F_ExecuteCommande(buffer_i2c_receive, index_buffer_rx);
//
//			if(retour == 0)
//			{
//				status_operation = E_STATUS_OK;
//			}
//			else
//			{
//				status_operation = E_STATUS_ERROR;
//			}
//*/
//			/*** Debut Section critique */
//		/*	NVIC_DisableIRQ(I2C2_EV_IRQn);
//			nb_data_i2c 	= 0;
//			index_buffer_rx = 0;
//			index_buffer_tx	= 0;
//			nb_data_timeout_i2c = 0;
//
//			/*** Fin section critique */
///*			NVIC_EnableIRQ(I2C2_EV_IRQn);
//
//		}*/
//
		// Si temps de realiser la Funny Action
		if(timer_1s >= 90)
		{
			// Ouvrir parasol
			LED_GREEN_ON();

		}

		if((timer_1s % 4) == 0)
		{
			LED_GREEN_TOGGLE();
			switch(pos)
			{
			case SERVO_0deg:
					pos = SERVO_90deg;
				break;
			case SERVO_90deg:
				pos = SERVO_180deg;
				break;
			case SERVO_180deg:
				pos = SERVO_0deg;
				break;
			default :
				pos = SERVO_0deg;
			}
			F_move_servo3(pos);
		}

	}

	return 0;
}


/**
 *
 */
void F_ExecuteCommande(uint8_t *buffer, uint8_t nb_data)
{
	// Vérifier commande
	switch(buffer[0])
	{
	case 1:	// Ouvrir le parasol
			if((buffer[1] == 0) && (buffer[2] == 0) && (buffer[3] == 0))
			{
				// Ouvrir le parasol

			}
		break;

	case 2:	// Faire tourner moteur DC (poissons)
			// Le parametre 1 est le moteur a commande (Droit ou gauche)
			// Le parametre 2 defini la montee ou la descente du bras
			// Si nombre de donnee est different de 0
				// definir vitesse
			// Sinon vitesse par defaut
		break;

	case 3:	// Envoyer la derniere mesure de distance(avant ou arriere)
		break;

	case 4:	// Controler servomoteur
		break;
	}

}



// ----------------------------------------------------------------------------
