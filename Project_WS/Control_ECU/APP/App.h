/*
 * App.h
 *
 *  Created on: Nov 1, 2021
 *      Author: omarh
 */

#ifndef APP_APP_H_
#define APP_APP_H_


#include "../std_types.h"

#define PASSWORD_LENGTH (uint8)2
#define PASSWORDS_MATCH 1
#define PASSWORDS_DIDNT_MATCH 0

#define GET_NEW_PASSWORD 1
#define MAIN_OPTIONS 2
#define OPEN_DOOR 3
#define CHANGE_PASSWORD 4

void App_init();
void App_start();
void App_receiveNewPassword();
uint8 App_confirmNewPassword(uint8 * password1, uint8 * password2);
void App_savePasswordInEEPROM();
void App_receiveLoginPassword();
uint8 App_verifyPassword(uint8 * loginPassword);

#endif /* APP_APP_H_ */
