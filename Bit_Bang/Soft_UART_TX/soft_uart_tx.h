/*
 * file version V0.0.0
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef SOFT_UART_TX_H_
#define SOFT_UART_TX_H_

#include "stdint.h"
#include "ring_buffer.h"

#define MAX_SOFT_UART 10

typedef enum Soft_UART_TX_State_t
{
    Idle,
    Send_Start_Bit,
    Send_Byte,
    Send_Parity_Bit,
    Send_Stop_Bit,

} Soft_UART_TX_State_t;

typedef struct Soft_UART_t
{
    Ring_Buffer_t *TX_Ring_Buffer;

    void (*TX_GPIO_Init)();

    void (*TX_High)();
    void (*TX_Low)();

    uint8_t TX_Bit_Count;
    uint8_t TX_Byte;

    uint8_t TX_Active_Flag;

    uint8_t TC_Flag; //transmission complete flag

    Soft_UART_TX_State_t State;

} Soft_UART_TX_t;

void Soft_UART_TX_Init();

void Soft_UART_TX_Add(Soft_UART_TX_t *uart_handle);

void Soft_UART_Send_Char(Soft_UART_TX_t *uart_handle, char data);

void Soft_UART_Send_String(Soft_UART_TX_t *uart_handle, char *str);

void Soft_UART_Start();

void Soft_UART_Stop();

void Soft_UART_Clear_TC_Flag(Soft_UART_TX_t *uart_handle);

uint8_t Soft_UART_Get_TC_Flag(Soft_UART_TX_t *uart_handle);

#endif /* SOFT_UART_TX_H_ */
