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

// Flag capture ultrason
uint8_t recuperer_ultrason;

// Flag Demande I2C
uint8_t demande_I2C;

/* Liste des points */
S_point liste_points_violet[] = {{.x = 0, .y = 0, .theta = 0},
							{.x = 650, .y = 900, .theta = 0},
							{.x = 700, .y = 1000, .theta = 0},
							{.x = 1100, .y = 1100, .theta = 0},
							{.x = 1250, .y = 950, .theta = 0},
							{.x = 1300, .y = 1300, .theta = 0},
							{.x = 1500, .y = 1550, .theta = 0},
							{.x = 1500, .y = 1850, .theta = 0},
							{.x = 1200, .y = 1650, .theta = 0},
							{.x = 900, .y = 1450, .theta = 0},
							{.x = 700, .y = 1550, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0}
};


S_point liste_points_vert[] = {{.x = 0, .y = 0, .theta = 0},
							{.x = 650, .y = 900, .theta = 0},
							{.x = 700, .y = 1000, .theta = 0},
							{.x = 1100, .y = 1100, .theta = 0},
							{.x = 1250, .y = 950, .theta = 0},
							{.x = 1300, .y = 1300, .theta = 0},
							{.x = 1500, .y = 1550, .theta = 0},
							{.x = 1500, .y = 1850, .theta = 0},
							{.x = 1200, .y = 1650, .theta = 0},
							{.x = 900, .y = 1450, .theta = 0},
							{.x = 700, .y = 1550, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0},
							{.x = 0, .y = 0, .theta = 0}
};


uint8_t F_Envoyer_commande(S_commande cmd);
uint8_t F_Recevoir_donnees_I2C(uint8_t nb_data, uint8_t *donnees);



int main()
{
	/* Variables */
	int16_t distance_obstacle_droit	= 50;
	int16_t distance_obstacle_gauche= 50;
	E_TRIG trigger_ultrason 	= TRIG_DROIT;
	recuperer_ultrason 			= 0;
	uint8_t flag_stop			= 1;

	S_point *liste_points 		= NULL;
	S_commande cmd;
	uint16_t index_point		= 0;

	E_SERVO_POSITION pos 		= SERVO_0deg;
	E_COULEUR couleur			= COULEUR_VERT;

	demande_I2C 				= 0;
	uint8_t etat_carte_moteur 	= 0;

	/*************/



	// Init IOs
	F_init_IO();

	// Initialiser les horloges
	F_init_clocks();

	// Init UART debug (2)
	F_init_UART_debug();

	// Init I2C
	F_init_I2C();

	// Initialiser les Timers
	F_Init_Timer();

	// Initialiser les capteurs ultrasons
	F_init_capteur_ultrasons();

	// Afficher version
	printf("Carte actionneur version v%d\r\n", VERSION);

	// Eteindre les LEDs
	LED_ORANGE_OFF();
	LED_GREEN_OFF();
	LED_RED_OFF();

	// Placer le parasol dans ca position initiale
	F_move_servo3(pos);

	// Attendre que la prise jack soit retiree
	// While(!(GPIOB->IDR & GPIOA_IDR14));

	// Recuperer la couleur du match
	if(GPIOA->IDR & GPIO_IDR_5)
	{
		liste_points = &liste_points_vert[0];
		couleur = COULEUR_VERT;
	}
	else
	{
		liste_points = &liste_points_violet[0];
		couleur = COULEUR_VERT;
	}

	while(1)
	{
		// Tester si un obstacle est present
		if(recuperer_ultrason == 1)
		{
			recuperer_ultrason = 0;	// reset flag
			LED_GREEN_OFF();
			if(trigger_ultrason == TRIG_DROIT)
			{
				distance_obstacle_droit = F_generer_trig(trigger_ultrason);
				trigger_ultrason = TRIG_GAUCHE;
			}
			else
			{
				distance_obstacle_gauche = F_generer_trig(trigger_ultrason);
				trigger_ultrason = TRIG_DROIT;
			}

			// Envoyer un signal a la carte moteur pour s'arreter
			if((distance_obstacle_droit <= 20) || (distance_obstacle_gauche <= 20))
			{
				if(flag_stop == 0)
				{
					LED_GREEN_ON();
					// allumer LED Rouge
					cmd.commande = CMD_START_STOP;
					cmd.param1 = 0;
					cmd.param2 = 0;
					cmd.param3 = 0;
					F_Envoyer_commande(cmd);	// Arreter le robot

					flag_stop = 1;
				}
			}
			else
			{
				if(flag_stop == 1)
				{
					// Eteindre LED Rouge
					LED_GREEN_OFF();
					cmd.commande = CMD_START_STOP;
					cmd.param1 = 1;
					cmd.param2 = 0;
					cmd.param3 = 0;
					F_Envoyer_commande(cmd);	// Faire repartir le robot
					flag_stop = 0;
				}
			}
		}


		// Si temps de realiser la Funny Action
		if(timer_1s >= 90)
		{
			LED_GREEN_ON();

			// Ouvrir parasol
			pos = SERVO_90deg;
			F_move_servo3(pos);
		}


		// Demander si bien arrive au point demande
	/*	if(	demande_I2C == 1)
		{
			F_Recevoir_donnees_I2C(1, &etat_carte_moteur);

			// Si le robot est a la arrive a destination
			if(etat_carte_moteur == 1)
			{
				// faire l'action des poissons
				if(couleur == COULEUR_VERT)
				{
					// Baisser le bras et activer l'electro-aimant
					if(index_point == 10)
					{

					}
					// Monter le bras
					else if(index_point == 11)
					{

					}
					// Descendre le bras et desactiver l'electro-aimant
					else if(index_point == 12)
					{

						// Monter le bras a la fin
					}
				}
				else
				{
					// Baisser le bras et activer l'electro-aimant
					if(index_point == 10)
					{

					}
					// Monter le bras
					else if(index_point == 11)
					{

					}
					// Descendre le bras et desactiver l'electro-aimant
					else if(index_point == 12)
					{

						// Monter le bras a la fin
					}


				}
				// Envoyer un nouveau point
				cmd.commande = CMD_GOTO;
				cmd.param1 = liste_points[index_point].x;
				cmd.param2 = liste_points[index_point].y;
				cmd.param3 = liste_points[index_point].theta;
				F_Envoyer_commande(cmd);
			}
			demande_I2C = 0;
		}

*/



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
		//	F_generer_trig(TRIG_AVANT);
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


	/*	if((timer_1s ) == 5)
		{
			LED_GREEN_TOGGLE();
			pos = SERVO_90deg;
			F_move_servo3(pos);
		}
		*/
	/*	EN_M1_ON();
		IN1_M1_ON();
		IN2_M1_OFF();
		while(GPIOA->IDR & GPIO_IDR_1);
		//EN_M1_OFF();
		IN1_M1_OFF();
		IN2_M1_OFF();
		LED_ORANGE_ON();
		while(!(GPIOA->IDR & GPIO_IDR_1));
*/
		//EN_M1_ON();
	/*	IN1_M1_OFF();
		IN2_M1_ON();
		//EN_M1_ON();
		//IN1_M1_OFF();
		//IN2_M1_ON();
		while(GPIOA->IDR & GPIO_IDR_0);
		//EN_M1_OFF();
		IN1_M1_ON();
		IN2_M1_ON();
		LED_ORANGE_ON();
		while(!(GPIOA->IDR & GPIO_IDR_0));

*/
		//LED_ORANGE_OFF();


		// Test switch arriere
		if(!(GPIOB->IDR & GPIO_IDR_13))
		{
			LED_GREEN_TOGGLE();
		}


	}

	return 0;
}


/**
 * Envoi d'une commande sur le bus I2C
 */
uint8_t F_Envoyer_commande(S_commande cmd)
{
	uint8_t retour = 1;
	uint8_t tab_a_envoyer[10] = {0};

	tab_a_envoyer[0] = cmd.commande;
	tab_a_envoyer[1] = (uint8_t)((cmd.param1 >> 8) & 0x00FF);
	tab_a_envoyer[2] = (uint8_t)(cmd.param1 & 0x00FF);
	tab_a_envoyer[3] = (uint8_t)((cmd.param2 >> 8) & 0x00FF);
	tab_a_envoyer[4] = (uint8_t)(cmd.param2 & 0x00FF);
	tab_a_envoyer[5] = (cmd.param3);


	retour = F_transmit_to_slave(LENGTH_CMD, tab_a_envoyer);

	return retour;
}


/**
 * Envoi d'une commande sur le bus I2C
 */
uint8_t F_Recevoir_donnees_I2C(uint8_t nb_data, uint8_t *donnees)
{
	uint8_t retour = 1;
	retour = F_receive_from_slave(nb_data, donnees);

	return retour;
}

// ----------------------------------------------------------------------------
