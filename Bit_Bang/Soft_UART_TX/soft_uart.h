#ifndef SOFT_UART_H_
#define SOFT_UART_H_

#include "stm32f1xx_hal.h"
#include "main.h"
#include "ring_buffer.h"

typedef enum Soft_UART_TX_State_t
    {
    Idle,
    Send_Start_Bit,
    Send_Byte,
    Send_Parity_Bit,
    Send_Stop_Bit,
    } Soft_Uart_TX_State_t;

typedef struct Soft_Uart_t
    {

	Ring_Buffer_t TX_Ring_Buffer;

	GPIO_TypeDef *GPIO_TX_Port;
	uint16_t GPIO_TX_Pin;

	uint8_t TX_Bit_Count;
	uint8_t TX_Byte;

	uint8_t TX_Active_Flag;

	uint8_t TC_Flag; //transmission complete flag

	Soft_Uart_TX_State_t State;

    } Soft_Uart_TX_t;

void Soft_Uart_TX_Init();
void Soft_Uart_TX_Add(Soft_Uart_TX_t *uart_handle);
void Soft_Uart_Send_Char(Soft_Uart_TX_t *uart_handle, char data);
void Soft_Uart_Send_String(Soft_Uart_TX_t *uart_handle, char *str);
void Soft_Uart_Start();
void Soft_Uart_Stop();
uint8_t Soft_Uart_Get_TC_Flag(Soft_Uart_TX_t *uart_handle);
void Soft_Uart_Clear_TC_Flag(Soft_Uart_TX_t *uart_handle);

#endif /* SOFT_UART_H_ */
