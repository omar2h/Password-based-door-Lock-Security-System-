/*
 * App.h
 *
 *  Created on: Nov 1, 2021
 *      Author: omarh
 */

#ifndef APP_CONTROL_H_
#define APP_CONTROL_H_


#include "../std_types.h"

#define PASSWORD_LENGTH (uint8)2
#define PASSWORDS_MATCH 1
#define PASSWORDS_DIDNT_MATCH 0
#define Control_START 0
#define GET_NEW_PASSWORD 1
#define MAIN_OPTIONS 2
#define LOGIN 3
#define OPEN_DOOR 4
#define CHANGE_PASSWORD 5
#define MAX_TRIALS 3
#define ALARM 7

void Control_init();
void Control_start();
void Control_receiveNewPassword();
uint8 Control_confirmNewPassword(uint8 * password1, uint8 * password2);
void Control_savePasswordInEEPROM();
void Control_mainOptions();
void Control_receiveLoginPassword(const uint8 option);
uint8 Control_verifyPassword(uint8 * loginPassword);
void Control_openDoor();
void Control_doorTimer();

#endif /* APP_CONTROL_H_ */
