#include "soft_uart_tx.h"
#include "stddef.h"

extern void Soft_UART_TX_TIM_Start();
extern void Soft_UART_TX_TIM_Stop();

static uint8_t Soft_UART_Count = 0;

Soft_UART_TX_t *Soft_UART_List[MAX_SOFT_UART];

void Soft_UART_TX_Add(Soft_UART_TX_t *uart_handle)
{
    if (Soft_UART_Count < MAX_SOFT_UART)
    {
        uart_handle->TX_Active_Flag = 0;
        uart_handle->TX_Bit_Count = 0;
        uart_handle->TC_Flag = 0;
        uart_handle->TX_Byte = 0;
        uart_handle->TX_GPIO_Init();
        Soft_UART_List[Soft_UART_Count] = uart_handle;
        Soft_UART_Count++;
    }
}

static void Soft_UART_TX_State_Machine(Soft_UART_TX_t *uart_handle)
{
    if (uart_handle->State == Idle)
    {
        if (Ring_Buffer_Get_Count(uart_handle->TX_Ring_Buffer))
        {
            uart_handle->TC_Flag = 0;
            uart_handle->TX_Bit_Count = 0;
            uart_handle->State = Send_Start_Bit;
            Ring_Buffer_Get_Char(uart_handle->TX_Ring_Buffer, &uart_handle->TX_Byte);
        }
        else
        {
            uart_handle->TX_Active_Flag = 0;
            uart_handle->TC_Flag = 1;
        }
    }
    else if (uart_handle->State == Send_Start_Bit)
    {
        uart_handle->TX_Low();
        uart_handle->State = Send_Byte;
    }
    else if (uart_handle->State == Send_Byte)
    {
        if (uart_handle->TX_Byte & 0x01)
        {
            uart_handle->TX_High();
        }
        else
        {
            uart_handle->TX_Low();
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
        uart_handle->TX_High();
        uart_handle->State = Idle;
    }
}

void Soft_UART_Timer_ISR()
{
    Soft_UART_TX_t *uart_handle = NULL;
    uint8_t uart_active_count = 0;

    for (uint8_t i = 0; i < Soft_UART_Count; i++)
    {
        /* grab uart handle from list*/
        uart_handle = Soft_UART_List[i];

        if (uart_handle->TX_Active_Flag)
        {
            uart_active_count++;
            Soft_UART_TX_State_Machine(uart_handle);
        }
    }

    // if all soft_uart are idle, disable tim interrupt
    if (!uart_active_count)
    {
        Soft_UART_TX_TIM_Stop();
    }
}

void Soft_UART_Send_Char(Soft_UART_TX_t *uart_handle, char data)
{
    Ring_Buffer_Put_Char(uart_handle->TX_Ring_Buffer, data);
    uart_handle->TX_Active_Flag = 1;
    Soft_UART_TX_TIM_Start();
}

void Soft_UART_Send_String(Soft_UART_TX_t *uart_handle, char *str)
{
    while (*str != '\0')
    {
        Ring_Buffer_Put_Char(uart_handle->TX_Ring_Buffer, *str++);
    }
    uart_handle->TX_Active_Flag = 1;
    Soft_UART_TX_TIM_Start();
}

uint8_t Soft_UART_Get_TC_Flag(Soft_UART_TX_t *uart_handle)
{
    return uart_handle->TC_Flag;
}

void Soft_UART_Clear_TC_Flag(Soft_UART_TX_t *uart_handle)
{
    uart_handle->TC_Flag = 0;
}
