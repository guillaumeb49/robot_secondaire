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
	volatile uint32_t i = 0;
	volatile uint32_t j = 0;
	// Init IOs
	F_init_IO();

	// Init UART debug (2)
	F_init_UART_debug();

	printf("Hello World\r\n");

	LED_GREEN_ON()
	LED_ORANGE_ON()

	// Try the L298N
	IN1_M1_OFF();
	IN2_M1_ON();
	EN_M1_ON();


	while(1)
	{
		for(i=0;i<8000;i++)
		{
			for(j=0;j<1000;j++);
		}
		LED_RED_TOGGLE()
	}

	return 0;
}

// ----------------------------------------------------------------------------
