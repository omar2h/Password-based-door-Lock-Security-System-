/*
 * Control.c
 *
 *  Created on: Nov 1, 2021
 *      Author: omarh
 */
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../ECUAL/lcd.h"
#include "../MCAL/uart.h"
#include "../MCAL/twi.h"
#include "../MCAL/timer.h"
#include "../ECUAL/external_eeprom.h"
#include "../ECUAL/dc_motor.h"
#include "Control.h"
uint8 g_doorPassword[5]={0};
uint8 state = Control_START;
static uint16 g_timerCount=0;

void Control_init(){
	SREG  |= (1<<7);
	TIMER_ConfigType TIMER_Configurations = {TIMER0, CTC, F_CPU_256, 0, 250};
	UART_ConfigType UART_Configurations = {BAUDRATE_9600, DATA_8BITS, NO_PARITY, ONE_STOP_BIT};
	Twi_ConfigType config = {TWI_BIT_RATE, TWI_PRESCALER, TWI_SLAVE_ADDRESS};

	TIMER_init(&TIMER_Configurations);
	UART_init(&UART_Configurations);
	TWI_init(&config);
	TIMER_setCallBack(Control_doorTimer);
	LCD_init();
	DcMotor_Init();
	Control_start();
}

void Control_start(){
	while(1){
		//state = UART_receiveByte();
		state = OPEN_DOOR;
		switch(state){
			case Control_START:
				UART_sendByte(Control_START);
				Control_receiveNewPassword();
				break;

			case GET_NEW_PASSWORD:
				Control_receiveNewPassword();
				break;

			case MAIN_OPTIONS:
				Control_mainOptions();
				break;

			case OPEN_DOOR:
				Control_openDoor();
				while(1);
				break;
		}
	}
}

void Control_receiveNewPassword(){
	uint8 i, key, confirm_password[PASSWORD_LENGTH] = {0};
	for(i=0; i<PASSWORD_LENGTH; i++){
		key = UART_receiveByte();
		g_doorPassword[i] = key;
		LCD_intgerToString(g_doorPassword[i]);
	}

	for(i=0; i<PASSWORD_LENGTH; i++){
		key = UART_receiveByte();
		confirm_password[i] = key;
		//UART_sendByte(g_doorPassword[i]);
		LCD_intgerToString(confirm_password[i]);
	}
	LCD_intgerToString(5);
	if(Control_confirmNewPassword(g_doorPassword, confirm_password) == PASSWORDS_MATCH){
		//LCD_intgerToString(1);
		Control_savePasswordInEEPROM();
		UART_sendByte(PASSWORDS_MATCH);
		state = MAIN_OPTIONS;
	}
	else{
		UART_sendByte(PASSWORDS_DIDNT_MATCH);
		state= GET_NEW_PASSWORD;
	}
	UART_sendByte(state);
}

uint8 Control_confirmNewPassword(uint8 * password1, uint8 * password2){
	uint8 i;
	for(i=0; i<PASSWORD_LENGTH; i++){
		if(password1[i] != password2[i]){
			return PASSWORDS_DIDNT_MATCH;
		}
	}
	return PASSWORDS_MATCH;
}

void Control_savePasswordInEEPROM(){
	uint8 i, val;
	for(i = 0; i<PASSWORD_LENGTH; i++){
		EEPROM_writeByte(0x0311 + i, g_doorPassword[i]);
		_delay_ms(10);
	}
	for(i = 0; i<PASSWORD_LENGTH; i++){
		EEPROM_readByte(0x0311 + i, &val);
		_delay_ms(10);
		LCD_intgerToString(val);
	}
	LCD_intgerToString(5);
}

void Control_mainOptions(){
	uint8 option;
	option = UART_receiveByte();
	Control_receiveLoginPassword(option);
}


void Control_receiveLoginPassword(const uint8 option){
	uint8 i, key, trials=0, loginPassword[PASSWORD_LENGTH] = {0};
	UART_sendByte(LOGIN);
	while(trials<MAX_TRIALS){
		for(i=0; i<PASSWORD_LENGTH; i++){
			key = UART_receiveByte();
			loginPassword[i] = key;
			LCD_intgerToString(loginPassword[i]);
		}

		if(Control_verifyPassword(loginPassword) == PASSWORDS_MATCH){
			UART_sendByte(PASSWORDS_MATCH);
			state = option;
			UART_sendByte(state);
			return;
		}
		else{
			UART_sendByte(PASSWORDS_DIDNT_MATCH);
			trials++;
			UART_sendByte(LOGIN);
		}
	}
	state = ALARM;
	UART_sendByte(ALARM);

}

uint8 Control_verifyPassword(uint8 * loginPassword){
	uint8 i, saved_password[PASSWORD_LENGTH] = {0};
	for(i = 0; i<PASSWORD_LENGTH; i++){
		EEPROM_readByte(0x0311 + i, &saved_password[i]);
		_delay_ms(10);
		if(loginPassword[i] != saved_password[i])
			return PASSWORDS_DIDNT_MATCH;
		}
	return PASSWORDS_MATCH;
}

void Control_openDoor(){
	g_timerCount= 0;
	LCD_intgerToString(55);
	DcMotor_Rotate(DC_MOTOR_CW);
	while(1){
		if(g_timerCount == 440){
			TIMER_deinit();
			DcMotor_Rotate(DC_MOTOR_STOP);
		}

	}
}

void Control_doorTimer(){
	g_timerCount++;
	if(g_timerCount == 123)
		DcMotor_Rotate(DC_MOTOR_STOP);
	else if(g_timerCount == 246)
		DcMotor_Rotate(DC_MOTOR_ACW);
}
