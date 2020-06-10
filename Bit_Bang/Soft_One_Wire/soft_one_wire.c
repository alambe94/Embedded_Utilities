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

#include "soft_one_wire.h"

extern Delay_Micros(uint16_t micros);

void Soft_OW_Init(Soft_OW_t *ow_handle)
{
    ow_handle->Pin_Init();
}

static void Soft_OW_Write_Bit(Soft_OW_t *ow_handle, uint8_t bit)
{
    ow_handle->Pin_Low();

    if (bit)
    {
        Delay_Micros(10);

        ow_handle->Pin_High();

        Delay_Micros(50);
    }
    else
    {
        Delay_Micros(50);

        ow_handle->Pin_High();

        Delay_Micros(10);
    }
}

static uint8_t Soft_OW_Read_Bit(Soft_OW_t *ow_handle)
{
    uint8_t xreturn = 0;

    ow_handle->Pin_Low();

    Delay_Micros(5);

    ow_handle->Pin_High();

    Delay_Micros(10);

    if (ow_handle->Pin_Read())
    {
        xreturn = 1;
    }

    Delay_Micros(45);

    return xreturn;
}

uint8_t Soft_OW_Reset(Soft_OW_t *ow_handle)
{
    uint8_t xreturn = 0;

    ow_handle->Pin_Low();

    /* wait 480 us*/
    Delay_Micros(500);

    ow_handle->Pin_High();

    /* wait 15-60µs*/
    Delay_Micros(60);

    if (!ow_handle->Pin_Read())
    {
        xreturn = 1;
    }

    Delay_Micros(250);

    return xreturn;
}

void Soft_OW_Send_Byte(Soft_OW_t *ow_handle, uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        Soft_OW_Write_Bit(ow_handle, (byte & 0x01));
        byte >>= 1;
    }
}

uint8_t Soft_OW_Receive_Byte(Soft_OW_t *ow_handle)
{
    uint8_t rx_data = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        rx_data >>= 1;

        if (Soft_OW_Read_Bit(ow_handle))
        {
            rx_data |= 0x80;
        }
    }

    return rx_data;
}
