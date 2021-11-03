 /******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Header file for the TWI(I2C) AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/ 

#ifndef TWI_H_
#define TWI_H_

#include "../std_types.h"

#define TWI_BIT_RATE 			(uint32)400000
#define TWI_PRESCALER 				(uint8)1
#define TWI_SLAVE_ADDRESS 		(uint8)0b00000010

#define TWI_TWBR_VALUE(bitrate, prescaler) (uint8_t)(((F_CPU/(bitrate))-16)/(2*prescaler))
enum{
	PRESCALER_1, PRESCALER_4, PRESCALER_16, PRESCALER_64
}TWI_prescaler;

typedef struct{
	uint32 bit_rate;
	uint8 prescaler;
	uint8 address;
}Twi_ConfigType;

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void TWI_init(Twi_ConfigType * config);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(uint8 data);
uint8 TWI_readByteWithACK(void);
uint8 TWI_readByteWithNACK(void);
uint8 TWI_getStatus(void);


#endif /* TWI_H_ */
