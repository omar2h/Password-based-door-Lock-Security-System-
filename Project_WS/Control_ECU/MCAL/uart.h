#ifndef UART_H_
#define UART_H_

#include "../std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* UART transmission speed configuration*/
#define NORMAL_SPEED 0
#define DOUBLE_SPEED 1
#define UART_RATE DOUBLE_SPEED

#if(UART_RATE == NORMAL_SPEED)
#define UBRR_CALC(baudrate) (uint16)((((F_CPU) / ((baudrate) * (16UL)))) - 1)
#else
#define UBRR_CALC(baudrate) (uint16)((((F_CPU) / ((baudrate) * (8UL)))) - 1)
#endif


#define BAUDRATE_1200		1200
#define BAUDRATE_2400		2400
#define BAUDRATE_4800		4800
#define BAUDRATE_9600		9600
#define BAUDRATE_14400	    14400
#define BAUDRATE_19200	    19200
#define BAUDRATE_38400	    38400
#define BAUDRATE_57600      57600

#define TEMP_END_OF_STRING '#'

#define UART_RATE DOUBLE_SPEED
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef uint32 UART_baudrate;

typedef enum{
	DATA_5BITS, DATA_6BITS, DATA_7BITS, DATA_8BITS, DATA_9BITS = 8
}UART_dataBits;

typedef enum{
	NO_PARITY, ODD_PARITY, EVEN_PARTIY
}UART_parityBit;

typedef enum{
	ONE_STOP_BIT, TWO_STOP_BIT
}UART_stopBits;

typedef struct{
	UART_baudrate baudrate;
	UART_dataBits data_bits;
	UART_parityBit parity_bit;
	UART_stopBits stop_bits;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
