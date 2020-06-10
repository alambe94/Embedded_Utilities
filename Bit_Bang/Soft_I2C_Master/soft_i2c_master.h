#include "stdint.h"
#include "ring_buffer.h"

#ifndef SOFT_I2C_MASTER_H_
#define SOFT_I2C_MASTER_H_

/** i2c transaction in polling or interrupt mode*/
#define I2C_USE_INTERRUPT_MODE 1
#define I2C_USE_BUFFERED_MODE 1
#define MAX_SOFT_I2C_MASTER 5

typedef enum Soft_I2C_Master_State_t
{
    Generate_Start,
    Data_Out,
    Read_ACK,
    Data_In,
    Give_ACK,
    Generate_Stop

} Soft_I2C_Master_State_t;

typedef enum Soft_I2C_Master_Flags_t
{
    I2C_Ok,
    I2C_Error,
    I2C_Busy

} Soft_I2C_Master_Flags_t;

typedef struct Soft_I2C_Master_t
{
    /** I2C TX ring buffer */
    Ring_Buffer_t *I2C_TX_Ring_Buffer;

    /** I2C RX ring buffer */
    Ring_Buffer_t *I2C_RX_Ring_Buffer;

    void (*Soft_I2C_GPIO_Init)();

    void (*Soft_I2C_SDA_High)();
    void (*Soft_I2C_SDA_Low)();
    void (*Soft_I2C_SCL_High)();
    void (*Soft_I2C_SCL_Low)();

    uint8_t (*Soft_I2C_SDA_Read)();

    /** slave address plus read/write bit */
    uint8_t Address_RW;

    uint16_t Bytes_To_Receive; /* no of bytes to receive in Master_Receive */

    uint8_t Bit_Count;
    uint8_t Current_Byte;

    uint8_t Active_Flag;

    /** track clock high low state */
    uint8_t CLK_State;

    /** track state machine  */
    Soft_I2C_Master_State_t State;

    Soft_I2C_Master_Flags_t Status_Flag;

    void (*callback)(Soft_I2C_Master_Flags_t flag);

} Soft_I2C_Master_t;

void Soft_I2C_Master_Init();

void Soft_I2C_Master_Add(Soft_I2C_Master_t *i2c_handle);

Soft_I2C_Master_Flags_t Soft_I2C_Master_Get_Status(Soft_I2C_Master_t *i2c_handle);

uint16_t Soft_I2C_Master_Available(Soft_I2C_Master_t *i2c_handle);

uint8_t Soft_I2C_Master_Read_Byte(Soft_I2C_Master_t *i2c_handle);

void Soft_I2C_Master_Transmit(Soft_I2C_Master_t *i2c_handle,
                              uint8_t slave_address,
                              uint16_t reg_addr,
                              uint8_t reg_addr_size,
                              uint8_t *bytes,
                              uint16_t len);

void Soft_I2C_Master_Receive(Soft_I2C_Master_t *i2c_handle,
                             uint8_t slave_address,
                             uint16_t reg_addr,
                             uint8_t reg_addr_size,
                             uint16_t len);

void Soft_I2C_Master_Scan(Soft_I2C_Master_t *i2c_handle, uint8_t slave_address);

#endif /* SOFT_I2C_MASTER_H_ */
