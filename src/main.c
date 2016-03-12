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


int main()
{
	// Init IOs
	F_init_IO();

	// Init UART debug (2)
	F_init_UART_debug();

	printf("Hello World\r\n");

	LED_GREEN_ON()
	LED_ORANGE_ON()

	while(1);

	return 0;
}

// ----------------------------------------------------------------------------
