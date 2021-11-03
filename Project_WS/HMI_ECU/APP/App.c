/*
 * App.c
 *
 *  Created on: Nov 1, 2021
 *      Author: omarh
 */


#include "App.h"
#include "../ECUAL/keypad.h"
#include <util/delay.h>

uint8 g_doorPassword[5]={0};
uint8 state = APP_START;
void App_init(){
	UART_ConfigType UART_Configurations = {BAUDRATE_9600, DATA_8BITS, NO_PARITY, ONE_STOP_BIT};

	UART_init(&UART_Configurations);
	LCD_init();
	App_start();

}

void App_start(){
	while(1){
		switch(state){
			case APP_START:
				App_getNewPassword();
				break;

			case GET_NEW_PASSWORD:
				App_getNewPassword();
				break;

			case MAIN_OPTIONS:
				App_mainOptions();
				break;
		}

	}
}

void App_getNewPassword(){
	uint8 key, i, confirm_password[PASSWORD_LENGTH]={0};
	UART_sendByte(1);
	LCD_clearScreen();
	LCD_displayString("Enter new pass");
	LCD_moveCursor(1,0);
	App_getPassword(g_doorPassword);

	App_sendPasswordViaUart(g_doorPassword);

	LCD_clearScreen();
	LCD_displayString("Enter pass again");
	LCD_moveCursor(1,0);
	App_getPassword(confirm_password);

	App_sendPasswordViaUart(confirm_password);

	key = UART_receiveByte();
	if(key == PASSWORDS_MATCH){
		LCD_clearScreen();
		LCD_displayString("Pass saved");
		_delay_ms(1000);
		state = MAIN_OPTIONS;
	}
	else{
		LCD_clearScreen();
		LCD_displayString("Pass didn't match");
		_delay_ms(1000);
		state = GET_NEW_PASSWORD;
	}
}

void App_getPassword(uint8 * password){
	uint8 i;
	LCD_sendCommand(LCD_CURSOR_BLINKING);
	for(i=0; i<PASSWORD_LENGTH; i++){
		password[i] = KEYPAD_getPressedKey();
		//UART_sendByte(g_doorPassword[i]);
		LCD_intgerToString(password[i]);
	}
	LCD_sendCommand(LCD_CURSOR_OFF);
}
void App_sendPasswordViaUart(uint8 * password){
	uint8 i;
	for(i=0; i<PASSWORD_LENGTH;i++){
		UART_sendByte(password[i]);
		_delay_ms(20);
		LCD_intgerToString(password[i]);
	}
}

void App_mainOptions(){
	uint8 option, key, i;
	LCD_clearScreen();
	while(1){
		LCD_displayString("+: Open door");
		LCD_moveCursor(1,0);
		LCD_displayString("-: Change pass");
		if(KEYPAD_getPressedKey() == '+'){
			option = OPEN_DOOR;
			break;
		}
		else if(KEYPAD_getPressedKey() == '-'){
			option = CHANGE_PASSWORD;
			break;
		}
		else
			option = INVALID_INPUT;
	}

	switch(option){
		case OPEN_DOOR:
			UART_sendByte(OPEN_DOOR);
			App_openDoor();
			break;
		case CHANGE_PASSWORD:
			break;
	}
}

void App_openDoor(){
	uint8 key, i, login_password[PASSWORD_LENGTH]={0};
	LCD_clearScreen();
	LCD_displayString("Enter pass");
	LCD_moveCursor(1,0);
	App_getPassword(login_password);

	App_sendPasswordViaUart(login_password);

	key = UART_receiveByte();
	if(key == PASSWORDS_MATCH){
		LCD_clearScreen();
		LCD_displayString("Logged in");
		_delay_ms(1000);
	}
}
