/*
 * timer.h
 *
 *  Created on: Nov 1, 2021
 *      Author: omarh
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "../std_types.h"

#define SET_TIMER0_NORMAL_MODE() TCCR0 = (1<<FOC0)
enum{
	TIMER0, TIMER1
}TIMER_Id;

enum{
	NORMAL, PWM, CTC, FAST_PWM
}TIMER_Mode;

enum{
	NO_CLK, F_CPU_CLOCK, F_CPU_8 , F_CPU_64 , F_CPU_256 , F_CPU_1024
}TIMER_Clock;

typedef struct{
	uint8 id;
	uint8 mode;
	uint8 clock;
	uint16 initial_value;
	uint16 compare_value;
}TIMER_ConfigType;

void TIMER_init(TIMER_ConfigType * config);
void TIMER_setCallBack(void(*a_ptr)(void));

#endif /* TIMER_H_ */
