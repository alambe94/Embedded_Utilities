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

#include "soft_spi.h"

void Soft_SPI_Transmit_Receive_Byte(Soft_SPI_t *spi_handle, uint8_t tx_data, uint8_t *rx_data)
{
    uint8_t rx_data_temp = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        /* accumulate rx data, left shift*/
        rx_data_temp <<= 1;

        if (tx_data & 0x80)
        {
            spi_handle->MOSI_Write(1);
        }
        else
        {
            spi_handle->MOSI_Write(0);
        }

        spi_handle->CLK_Write(1);

        if (spi_handle->MISO_Read())
        {
            rx_data_temp |= 0x01;
        }

        spi_handle->CLK_Write(0);

        /*left shift to transmit next bit*/
        tx_data <<= 1;
    }

    *rx_data = rx_data_temp;
}

void Soft_SPI_Transmit_Bytes(Soft_SPI_t *spi_handle, uint8_t *buffer, uint16_t len)
{
    uint8_t rx_data;
    uint8_t tx_data = 0;

    while (len--)
    {
        tx_data = *buffer++;
        Soft_SPI_Transmit_Receive_Byte(spi_handle, tx_data, &rx_data);
    }
}

void Soft_SPI_Receive_Bytes(Soft_SPI_t *spi_handle, uint8_t *rx_buffer, uint16_t len)
{
    uint8_t tx_data = 0XFF; // dummy byte
    uint8_t rx_data;

    while (len--)
    {
        Soft_SPI_Transmit_Receive_Byte(spi_handle, tx_data, &rx_data);
        *rx_buffer = rx_data;
        rx_buffer++;
    }
}
