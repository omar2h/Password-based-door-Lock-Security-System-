/*
 * App.h
 *
 *  Created on: Nov 1, 2021
 *      Author: omarh
 */

#ifndef APP_APP_H_
#define APP_APP_H_

#include "../ECUAL/lcd.h"
#include "../MCAL/uart.h"
#include "../std_types.h"

#define PASSWORD_LENGTH (uint8)2
#define PASSWORDS_MATCH 1
#define PASSWORDS_DIDNT_MATCH 0
#define APP_START 0
#define GET_NEW_PASSWORD 1
#define MAIN_OPTIONS 2
#define OPEN_DOOR 3
#define CHANGE_PASSWORD 4
#define INVALID_INPUT 0

void App_init();
void App_start();
void App_getNewPassword();
void App_getPassword(uint8 * password);
void App_sendPasswordViaUart(uint8 * password);
void App_mainOptions();
void App_openDoor();

#endif /* APP_APP_H_ */
