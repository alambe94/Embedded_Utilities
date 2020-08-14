/** Non blocking stm32 uart tx and rx stream with dma */

#include <stdarg.h>

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
 * @brief send character
 * @param data char to be sent
 */
void UART_Send_Char(char data)
{
    RB_Put_Char(&UART_TX_RB, data);

    /** uart is not transmitting */
    if (HAL_UART_GetState(&huart1) & HAL_USART_STATE_BUSY_TX != HAL_USART_STATE_BUSY_TX)
    {
        /** call complete ISR manually to trigger start of new transaction*/
        UART_TX_CMPLT_ISR();
    }
}

/**
 * @brief send bytes buffer
 * @param data input buffer
 * @param number of chars to send
 **/
void UART_Send_Bytes(char *data, uint32_t count)
{
    RB_Put_Chars(&UART_TX_RB, data, count);

    /** uart is not transmitting */
    if (HAL_UART_GetState(&huart1) & HAL_USART_STATE_BUSY_TX != HAL_USART_STATE_BUSY_TX)
    {
        /** call complete ISR manually to trigger start of new transaction*/
        UART_TX_CMPLT_ISR();
    }
}

/**
 * @brief send null terminated string
 * @param data input buffer
 * @param number of chars to send
 **/
void UART_Send_String(char *str)
{
    while (*str)
    {
        RB_Put_Char(&UART_TX_RB, *str++);
    }

    /** uart is not transmitting */
    if (HAL_UART_GetState(&huart1) & HAL_USART_STATE_BUSY_TX != HAL_USART_STATE_BUSY_TX)
    {
        /** call complete ISR manually to trigger start of new transaction*/
        UART_TX_CMPLT_ISR();
    }
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

    RB_Put_Chars(&UART_TX_RB, buffer, count, 1000);

    /** uart is not transmitting */
    if (HAL_UART_GetState(&huart1) & HAL_USART_STATE_BUSY_TX != HAL_USART_STATE_BUSY_TX)
    {
        /** call complete ISR manually to trigger start of new transaction*/
        UART_TX_CMPLT_ISR();
    }
}

/**
 * @brief number of chars received in uart buffer
 */
uint32_t UART_Get_Count()
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
uint32_t UART_Get_Chars(uint8_t *buffer, uint32_t count, uint32_t timeout)
{
    return RB_Get_Chars(&UART_RX_RB, buffer, count, timeout);
}

/**
 * @brief get chars upto '\r'
 * @param timeout
 * @retval number chars received
 */
uint32_t UART_Get_Line(char *buffer, uint32_t timeout)
{
    uint32_t tick_now = HAL_GetTick();
    uint32_t tick_timeout = tick_now + timeout;
    uint32_t rx_chars_cnt = 0;
    int rx_char;

    while (tick_now < tick_timeout)
    {
        tick_now = HAL_GetTick();
        rx_char = UART_Get_Char(1);
        if (rx_char != -1)
        {
            /** carriage return found */
            if (rx_char == '\r')
            {
                buffer[rx_chars_cnt] = '\0';
                break;
            }
            if (rx_char != '\n') /** ignore '\n' if any */
            {
                buffer[rx_chars_cnt++] = rx_char;
            }
        }
    }

    return rx_chars_cnt;
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
        for (uint32_t i = 0; i < count; i++)
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