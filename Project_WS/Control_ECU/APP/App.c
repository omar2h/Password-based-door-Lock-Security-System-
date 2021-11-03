/*
 * App.c
 *
 *  Created on: Nov 1, 2021
 *      Author: omarh
 */
#include <util/delay.h>
#include "App.h"
#include "../ECUAL/lcd.h"
#include "../MCAL/uart.h"
#include "../MCAL/twi.h"
#include "../ECUAL/external_eeprom.h"
uint8 g_doorPassword[5]={0};
uint8 state;

void App_init(){
	UART_ConfigType UART_Configurations = {BAUDRATE_9600, DATA_8BITS, NO_PARITY, ONE_STOP_BIT};
	Twi_ConfigType config = {TWI_BIT_RATE, TWI_PRESCALER, TWI_SLAVE_ADDRESS};

	UART_init(&UART_Configurations);
	TWI_init(&config);
	LCD_init();
	App_start();
}

void App_start(){
	while(1){
		state = UART_receiveByte();
		switch(state){
			case GET_NEW_PASSWORD:
				App_receiveNewPassword();
				break;
			case OPEN_DOOR:
				App_receiveLoginPassword();
				break;

		}
	}
}

void App_receiveNewPassword(){
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
	if(App_confirmNewPassword(g_doorPassword, confirm_password) == PASSWORDS_MATCH){
		//LCD_intgerToString(1);
		App_savePasswordInEEPROM();
		UART_sendByte(PASSWORDS_MATCH);
	}
	else{
		UART_sendByte(PASSWORDS_DIDNT_MATCH);
	}


}

uint8 App_confirmNewPassword(uint8 * password1, uint8 * password2){
	uint8 i;
	for(i=0; i<PASSWORD_LENGTH; i++){
		if(password1[i] != password2[i]){
			return PASSWORDS_DIDNT_MATCH;
		}
	}
	return PASSWORDS_MATCH;
}

void App_savePasswordInEEPROM(){
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

void App_receiveLoginPassword(){
	uint8 i, key, loginPassword[PASSWORD_LENGTH] = {0};
	for(i=0; i<PASSWORD_LENGTH; i++){
		key = UART_receiveByte();
		loginPassword[i] = key;
		LCD_intgerToString(loginPassword[i]);
	}
	LCD_intgerToString(5);
	if(App_verifyPassword(loginPassword) == PASSWORDS_MATCH){
		UART_sendByte(PASSWORDS_MATCH);
	}
	else
		UART_sendByte(PASSWORDS_DIDNT_MATCH);
}

uint8 App_verifyPassword(uint8 * loginPassword){
	uint8 i, saved_password[PASSWORD_LENGTH] = {0};
	for(i = 0; i<PASSWORD_LENGTH; i++){
		EEPROM_readByte(0x0311 + i, &saved_password[i]);
		_delay_ms(10);
		if(loginPassword[i] != saved_password[i])
			return PASSWORDS_DIDNT_MATCH;
		}
	return PASSWORDS_MATCH;
}
