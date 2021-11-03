/*
 * timer.c
 *
 *  Created on: Nov 1, 2021
 *      Author: omarh
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect) {
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

void TIMER_init(TIMER_ConfigType * config){
	switch(config->id){
		case TIMER0:
			switch(config->mode){
				case NORMAL:
					TCNT0 = config->initial_value;
					TCCR0 = (1<<FOC0) | (config->clock<<CS00);
					TIMSK = (1<<TOIE0);

					break;

				case PWM:
					break;

				case CTC:
					TCNT0 = config->initial_value;
					OCR0 = config->compare_value;
					TCCR0 = (1<<FOC0) | (1<<WGM01) | (config->clock<<CS00);
					TIMSK |= (1<<OCIE0);
					break;

				case FAST_PWM:
					break;
			}
			break;
		case TIMER1:
			break;
	}
}

