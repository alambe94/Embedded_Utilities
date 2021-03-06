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

#ifndef SOFT_SPI_MASTER_H_
#define SOFT_SPI_MASTER_H_

/** std includes */
#include <stdint.h>

typedef struct Soft_SPI_t
{
    void (*SPI_GPIO_Init)(void);
    void (*CLK_Write)(uint8_t state);
    void (*MOSI_Write)(uint8_t state);
    uint8_t (*MISO_Read)(void);
} Soft_SPI_t;

void Soft_SPI_Transmit_Receive_Byte(Soft_SPI_t *spi_handle, uint8_t tx_data, uint8_t *rx_data);
void Soft_SPI_Transmit_Bytes(Soft_SPI_t *spi_handle, uint8_t *tx_buffer, uint16_t len);
void Soft_SPI_Receive_Bytes(Soft_SPI_t *spi_handle, uint8_t *rx_buffer, uint16_t len);

#endif /* SOFT_SPI_MASTER_H_ */
