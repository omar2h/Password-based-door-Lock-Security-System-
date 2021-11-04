/*
 * HMI.h
 *
 *  Created on: Nov 1, 2021
 *      Author: omarh
 */

#ifndef APP_HMI_H_
#define APP_HMI_H_

#include "../std_types.h"
#define EIGHTEEN_SECONDS 576
#define FIFTEEN_SECONDS 32
#define PASSWORD_LENGTH (uint8)2
#define PASSWORDS_MATCH 1
#define PASSWORDS_DIDNT_MATCH 0
#define HMI_START 0
#define GET_NEW_PASSWORD 1
#define MAIN_OPTIONS 2
#define LOGIN 3
#define OPEN_DOOR 4
#define CHANGE_PASSWORD 5
#define INVALID_INPUT 0

void HMI_init();
void HMI_start();
void HMI_getNewPassword();
void HMI_getPassword(uint8 * password);
void HMI_sendPasswordViaUart(uint8 * password);
void HMI_mainOptions();
void HMI_login();
void HMI_openDoor();
void HMI_doorTimer();

#endif /* APP_HMI_H_ */
