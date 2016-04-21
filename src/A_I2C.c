/*
 * A_I2C.c
 *
 *  Created on: 19 avr. 2016
 *      Author: guillaume
 */

#include "A_I2C.h"

uint8_t F_ExecuterCommande(uint8_t *buffer_reception)
{
	// Vérifier commande
	switch(buffer_reception[0])
	{
	case 1:	// Ouvrir le parasol
			if((buffer_reception[1] == 0) && (buffer_reception[2] == 0) && (buffer_reception[3] == 0))
			{
				// ouvrir le parasol

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

	return 0;
}
