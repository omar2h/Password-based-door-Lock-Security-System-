/*
 * HMI.c
 *
 *  Created on: Nov 1, 2021
 *      Author: omarh
 */


#include "HMI.h"

#include "../ECUAL/keypad.h"
#include "../ECUAL/lcd.h"
#include "../MCAL/uart.h"
#include "../MCAL/timer.h"
#include <util/delay.h>
#include <avr/interrupt.h>

uint8 g_doorPassword[5]={0};
uint8 state;
static uint16 g_timerCount=0;

void HMI_init(){
	SREG  |= (1<<7);
	TIMER_ConfigType TIMER_Configurations = {TIMER0, CTC, F_CPU_256, 0, 250};
	UART_ConfigType UART_Configurations = {BAUDRATE_9600, DATA_8BITS, NO_PARITY, ONE_STOP_BIT};
	TIMER_init(&TIMER_Configurations);
		TIMER_setCallBack(HMI_doorTimer);
	UART_init(&UART_Configurations);
	LCD_init();
	HMI_start();


}

void HMI_start(){
	while(1){
		state = UART_receiveByte();
		//state = OPEN_DOOR;
		switch(state){
			case HMI_START:
				HMI_getNewPassword();
				break;

			case GET_NEW_PASSWORD:
				HMI_getNewPassword();
				break;

			case MAIN_OPTIONS:
				HMI_mainOptions();
				break;

			case LOGIN:
				HMI_login();
				break;

			case OPEN_DOOR:
				HMI_openDoor();
				//while(1);
				break;

			case CHANGE_PASSWORD:
				break;
		}

	}
}

void HMI_getNewPassword(){
	uint8 key, confirm_password[PASSWORD_LENGTH]={0};
	//UART_sendByte(1);
	LCD_clearScreen();
	LCD_displayString("Enter new pass");
	LCD_moveCursor(1,0);
	HMI_getPassword(g_doorPassword);

	HMI_sendPasswordViaUart(g_doorPassword);

	LCD_clearScreen();
	LCD_displayString("Enter pass again");
	LCD_moveCursor(1,0);
	HMI_getPassword(confirm_password);

	HMI_sendPasswordViaUart(confirm_password);

	key = UART_receiveByte();
	if(key == PASSWORDS_MATCH){
		LCD_clearScreen();
		LCD_displayString("Pass saved");
		_delay_ms(1000);
	}
	else{
		LCD_clearScreen();
		LCD_displayString("Pass didn't match");
		_delay_ms(1000);
	}
}

void HMI_getPassword(uint8 * password){
	uint8 i;
	LCD_sendCommand(LCD_CURSOR_BLINKING);
	for(i=0; i<PASSWORD_LENGTH; i++){
		password[i] = KEYPAD_getPressedKey();
		//UART_sendByte(g_doorPassword[i]);
		LCD_intgerToString(password[i]);
	}
	LCD_sendCommand(LCD_CURSOR_OFF);
}
void HMI_sendPasswordViaUart(uint8 * password){
	uint8 i;
	for(i=0; i<PASSWORD_LENGTH;i++){
		UART_sendByte(password[i]);
		_delay_ms(20);
		LCD_intgerToString(password[i]);
	}
}

void HMI_mainOptions(){
	uint8 option, key;
	LCD_clearScreen();

	LCD_displayString("+: Open door");
	LCD_moveCursor(1,0);
	LCD_displayString("-: Change pass");
	while(1){
		key = KEYPAD_getPressedKey();
		if(key == '+'){
			option = OPEN_DOOR;
			break;
		}
		else if(key == '-'){
			option = CHANGE_PASSWORD;
			break;
		}
	}
	UART_sendByte(option);
}

void HMI_login(){
	uint8 key, login_password[PASSWORD_LENGTH]={0};
	LCD_clearScreen();
	LCD_displayString("Enter pass");
	LCD_moveCursor(1,0);
	HMI_getPassword(login_password);

	HMI_sendPasswordViaUart(login_password);

	key = UART_receiveByte();
	LCD_clearScreen();
	if(key == PASSWORDS_MATCH){
		LCD_displayString("Logged in");

	}
	else if(key == PASSWORDS_DIDNT_MATCH){
		LCD_displayString("Incorrect pass");
	}
	//_delay_ms(1000);
}

void HMI_openDoor(){
	LCD_clearScreen();
	LCD_displayString("Unlocking door...");
	g_timerCount = 0;
	while(1){
		if(g_timerCount == 300){
			TIMER_deinit();
			break;
		}
	}
}

void HMI_doorTimer(){
	g_timerCount++;

	if(g_timerCount == 100){
		LCD_clearScreen();
		LCD_displayString("Door is open...");
	}
	else if(g_timerCount == 200){
		LCD_clearScreen();
		LCD_displayString("Locking door...");
	}
}
