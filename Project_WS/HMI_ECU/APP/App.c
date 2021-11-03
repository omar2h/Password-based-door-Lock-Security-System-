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
void App_init(){
	UART_ConfigType UART_Configurations = {BAUDRATE_9600, DATA_8BITS, NO_PARITY, ONE_STOP_BIT};

	UART_init(&UART_Configurations);
	LCD_init();
	App_start();

}

void App_start(){
	while(1){
		App_getNewPassword();
	}
}

void App_getNewPassword(){
	UART_sendByte(1);
	uint8 i, confirm_password[PASSWORD_LENGTH]={0};
	LCD_displayString("Enter new pass");
	LCD_moveCursor(1,0);
	LCD_sendCommand(LCD_CURSOR_BLINKING);

	for(i=0; i<PASSWORD_LENGTH; i++){
		g_doorPassword[i] = KEYPAD_getPressedKey();
		//UART_sendByte(g_doorPassword[i]);
		LCD_intgerToString(g_doorPassword[i]);
	}

	App_sendPasswordViaUart(g_doorPassword);

	LCD_clearScreen();
	LCD_displayString("Enter pass again");
	for(i=0; i<PASSWORD_LENGTH; i++){
		confirm_password[i] = KEYPAD_getPressedKey();
		LCD_intgerToString(confirm_password[i]);
	}
	App_sendPasswordViaUart(confirm_password);
}

void App_sendPasswordViaUart(uint8 * password){
	uint8 i;
	for(i=0; i<PASSWORD_LENGTH;i++){
		UART_sendByte(password[i]);
		_delay_ms(10);
	}
}