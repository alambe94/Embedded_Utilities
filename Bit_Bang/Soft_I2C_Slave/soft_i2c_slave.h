#include "stm32f4xx_hal.h"

#ifndef SOFT_I2C_SLAVE_H_
#define SOFT_I2C_SLAVE_H_

#define SOFT_I2C_OK		0
#define SOFT_I2C_ERR		1

enum Soft_I2C_Slave_State_t
    {
    Slave_Detect_Start,
    Slave_Address_In,
    Slave_Data_In,
    Slave_Give_ACK,
    Slave_Data_Out,
    Slave_Read_ACK,
    Slave_Detect_Stop
    };

typedef enum Soft_I2C_Slave_Event_t
    {
    Slave_Start_Detected,
    Slave_Read_Request,
    Slave_Write_Request,
    Slave_ACK_Received,
    Slave_NACK_Received,
    Slave_Stop_Detected,
    } Soft_I2C_Slave_Event_t;

struct Soft_I2C_Slave_t
    {
	uint8_t *Buffer; /* I2C working buffer*/
	uint16_t Buffer_Index; /* Buffer index*/

	/* no of bytes slave is ready to receive or send if set 0 NACK will be send*/
	/* callback will be called when address is matched with I2C_Read_Request or I2C_Write_Request*/
	/* in callback set the buffer where the data will be written to or send from*/
	/* Byte_Count , slave is able to receive or transmit*/
	uint16_t Byte_Count;

	uint8_t Own_Address;

	uint8_t Received_Address;

	uint8_t Bit_Count;
	uint8_t Current_Byte;

	uint8_t Active_Flag;

	enum Soft_I2C_Slave_State_t state;

	void (*callback)(Soft_I2C_Slave_Event_t event);

    };
/* export i2c slave handle*/
extern struct Soft_I2C_Slave_t Soft_I2C_Slave;

void Soft_I2C_Slave_Init(void (*Event_Callback)(Soft_I2C_Slave_Event_t event));

#endif /* SOFT_I2C_SLAVE_H_ */
