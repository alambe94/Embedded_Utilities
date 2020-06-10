#include "soft_uart.h"

#define SOFT_UART_TIM htim4
#define MAX_SOFT_UART  10

extern TIM_HandleTypeDef htim4;

/*
 * TIM period value at 1us resolution
 * 9600   -> (104 - 1) CPU Clock >= 8MHz
 * 19200  -> (52 - 1)  CPU Clock >= 16MHz
 * 38400  -> (26 - 1)  CPU Clock >= 48MHz
 * 57600  -> (18 - 1)  CPU Clock >= 48MHz
 * 115200 -> (9 - 1)   CPU Clock >= 72MHz
 */

static uint8_t Soft_Uart_Count = 0;

Soft_Uart_TX_t *Soft_Uart_List[MAX_SOFT_UART];

void Soft_Uart_TX_Init()
    {
    /*tim configured in cube see tim.c*/
    }

void Soft_Uart_TX_Add(Soft_Uart_TX_t *uart_handle)
    {
    /**** TIM configured in cube to generate interrupt every 1/baud rate see tim.c*********/

    if (Soft_Uart_Count < MAX_SOFT_UART)
	{

	uart_handle->TX_Active_Flag = 0;
	uart_handle->TX_Bit_Count = 0;
	uart_handle->TC_Flag = 0;
	uart_handle->TX_Byte = 0;
	uart_handle->TX_Ring_Buffer.Read_Index = 0;
	uart_handle->TX_Ring_Buffer.Write_Index = 0;

	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//__HAL_RCC_GPIOB_CLK_ENABLE();
	//__HAL_RCC_GPIOC_CLK_ENABLE();
	//__HAL_RCC_GPIOD_CLK_ENABLE();
	//__HAL_RCC_GPIOE_CLK_ENABLE();
	//__HAL_RCC_GPIOF_CLK_ENABLE();

	/*Configure GPIO pin Output Level */

	HAL_GPIO_WritePin(uart_handle->GPIO_TX_Port, uart_handle->GPIO_TX_Pin,
		GPIO_PIN_SET);

	GPIO_InitStruct.Pin = uart_handle->GPIO_TX_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(uart_handle->GPIO_TX_Port, &GPIO_InitStruct);

	Soft_Uart_List[Soft_Uart_Count] = uart_handle;
	Soft_Uart_Count++;
	}
    }

static void Soft_Uart_TX_High(Soft_Uart_TX_t *uart_handle)
    {
    HAL_GPIO_WritePin(uart_handle->GPIO_TX_Port, uart_handle->GPIO_TX_Pin,
	    GPIO_PIN_SET);
    }

static void Soft_Uart_TX_Low(Soft_Uart_TX_t *uart_handle)
    {
    HAL_GPIO_WritePin(uart_handle->GPIO_TX_Port, uart_handle->GPIO_TX_Pin,
	    GPIO_PIN_RESET);
    }

static void Soft_Uart_TX_State_Machine(Soft_Uart_TX_t *uart_handle)
    {

    if (uart_handle->State == Idle)
	{

	if (Ring_Buffer_Get_Count(&uart_handle->TX_Ring_Buffer))
	    {
	    uart_handle->TC_Flag = 0;
	    uart_handle->TX_Bit_Count = 0;
	    uart_handle->State = Send_Start_Bit;
	    Ring_Buffer_Get_Char(&uart_handle->TX_Ring_Buffer,
		    (char*)&uart_handle->TX_Byte);
	    }
	else
	    {
	    uart_handle->TX_Active_Flag = 0;
	    uart_handle->TC_Flag = 1;
	    }

	}
    else if (uart_handle->State == Send_Start_Bit)
	{
	Soft_Uart_TX_Low(uart_handle);
	uart_handle->State = Send_Byte;
	}
    else if (uart_handle->State == Send_Byte)
	{

	if (uart_handle->TX_Byte & 0x01)
	    {
	    Soft_Uart_TX_High(uart_handle);
	    }
	else
	    {
	    Soft_Uart_TX_Low(uart_handle);
	    }

	uart_handle->TX_Byte >>= 1;
	uart_handle->TX_Bit_Count++;

	if (uart_handle->TX_Bit_Count == 8)
	    {
	    uart_handle->TX_Bit_Count = 0;
	    uart_handle->State = Send_Stop_Bit;
	    }

	}
    else if (uart_handle->State == Send_Parity_Bit)
	{
	/*not implemented yet*/
	}
    else if (uart_handle->State == Send_Stop_Bit)
	{
	Soft_Uart_TX_High(uart_handle);
	uart_handle->State = Idle;
	}

    }


/**** TIM configured in cube to generate interrupt every 1/baud rate *********/

void Soft_Uart_TIM_ISR()
    {

    Soft_Uart_TX_t *uart_handle = NULL;
    uint8_t uart_active_count = 0;

    for (uint8_t i = 0; i < Soft_Uart_Count; i++)
	{

	/* grab uart handle from list*/
	uart_handle = Soft_Uart_List[i];

	if (uart_handle->TX_Active_Flag)
	    {

	    uart_active_count++;

	    Soft_Uart_TX_State_Machine(uart_handle);
	    }

	}

    // if all soft_uart are idle, disable tim interrupt
    if (!uart_active_count)
	{
	HAL_TIM_Base_Stop_IT(&SOFT_UART_TIM);
	}

    }

void Soft_Uart_Send_Char(Soft_Uart_TX_t *uart_handle, char data)
    {
    Ring_Buffer_Put_Char(&uart_handle->TX_Ring_Buffer, data);
    uart_handle->TX_Active_Flag = 1;
    HAL_TIM_Base_Start_IT(&SOFT_UART_TIM);
    }

void Soft_Uart_Send_String(Soft_Uart_TX_t *uart_handle, char *str)
    {
    while (*str != '\0')
	{
	Ring_Buffer_Put_Char(&uart_handle->TX_Ring_Buffer, *str++);
	}
    uart_handle->TX_Active_Flag = 1;
    HAL_TIM_Base_Start_IT(&SOFT_UART_TIM);
    }

uint8_t Soft_Uart_Get_TC_Flag(Soft_Uart_TX_t *uart_handle)
    {
    return uart_handle->TC_Flag;
    }

void Soft_Uart_Clear_TC_Flag(Soft_Uart_TX_t *uart_handle)
    {
    uart_handle->TC_Flag = 0;
    }

void Soft_Uart_Start()
    {
    HAL_TIM_Base_Start_IT(&SOFT_UART_TIM);
    }

void Soft_Uart_Stop()
    {
    HAL_TIM_Base_Stop_IT(&SOFT_UART_TIM);
    }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
    {
    if (htim == &SOFT_UART_TIM)
	{
	Soft_Uart_TIM_ISR();
	}
    }
