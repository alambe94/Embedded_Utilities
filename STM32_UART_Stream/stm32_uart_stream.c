/** Non blocking stm32 uart tx and rx stream with dma */

#include<stdarg.h>

#include "rb.h"

static RB_t UART_TX_RB;
static RB_t UART_RX_RB;

#define RB_STORAGE_SIZE 128
static uint8_t RB_TX_Sorage[RB_STORAGE_SIZE];
static uint8_t RB_RX_Sorage[RB_STORAGE_SIZE];

void UART_Init(void)
{
    /** tx configured in cube with normal dma mode */
    /** rx configured in cube with cicular dma mode */

    /** init ring buffer */
    RB_Init(&UART_TX_RB, RB_TX_Sorage, RB_STORAGE_SIZE);
    RB_Init(&UART_RX_RB, RB_RX_Sorage, RB_STORAGE_SIZE);

    /** Enable idle interrupt */
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
}

/**
 * @brief wrapper printf around uart
 * @param fmt formatted string
 **/
void UART_Printf(const char *fmt, ...)
{
    static uint8_t buffer[128];

    va_list args;
    va_start(args, fmt);
    uint32_t count = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    /** uart is not transmitting */
    if (HAL_USART_GetState(&huart1) != HAL_USART_STATE_BUSY_TX)
    {
        /** start new transaction*/
        HAL_USART_Transmit_DMA(&huart1, buffer, count);
    }
    else
    {
        RB_Put_Chars(&UART_TX_RB, buffer, count, 1000);
    }

}

/**
 * @brief number of chars received in uart
 */
uint16_t UART_Get_Count()
{
    RB_Get_Count(&UART_RX_RB);
}

/**
 * @brief get single char 
 * retval -1 if no char
 */
int UART_Get_Char()
{
    RB_Get_Char(&UART_RX_RB);
}

/**
 * @brief get number chars 
 * retval 0 if no char
 */
uint16_t UART_Get_Chars(uint8_t *buffer, uint16_t count)
{
    return RB_Get_Chars(&UART_RX_RB, buffer, count, 1000);
}

/**************************** ISR ***********************/

/** call from HAL_UART_TxCpltCallback in stm32fxxx_it.c*/
void UART_TX_CMPLT_ISR()
{
    static uint8_t buffer[128];
    uint32_t count = RB_Get_Count(&UART_TX_RB);

    /** if ring buffer in not empty, start next transaction */
    if (count)
    {
        for (uint16_t i=0; i<count; i++)
        {
            buffer[i] = RB_Get_Char(&UART_TX_RB);
        }

        HAL_USART_Transmit_DMA(&huart1, buffer, count);
    }

}

/** call from USARTx_IRQHandler in stm32fxxx_it.c*/
void UART_Idle_ISR()
{
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);

        /** data is written to buffer via uart DMA in background*/
        /** need to update Write_Index manually */
        UART_RX_RB.RB_Write_Index = UART_RX_RB.RB_Size - huart1.hdmarx->Instance->CNDTR;
    }
}