#ifndef SOFT_SPI_H_
#define SOFT_SPI_H_

#include "stm32f1xx_hal.h"
#include "main.h"

typedef struct Soft_SPI_t
    {

	GPIO_TypeDef* GPIO_SCK_Port;
	uint16_t GPIO_SCK_Pin;

	GPIO_TypeDef* GPIO_MOSI_Port;
	uint16_t GPIO_MOSI_Pin;

	GPIO_TypeDef* GPIO_MISO_Port;
	uint16_t GPIO_MISO_Pin;

    } Soft_SPI_t;

void Soft_SPI_Init(Soft_SPI_t* handle);
void Soft_SPI_Send_Receive_Byte(Soft_SPI_t* spi_handle, uint8_t tx_data, uint8_t* rx_data);
void Soft_SPI_Send_Bytes(Soft_SPI_t* spi_handle, uint8_t* tx_buffer, uint16_t len);
void Soft_SPI_Receive_Bytes(Soft_SPI_t* spi_handle, uint8_t* rx_buffer, uint16_t len);

#endif /* SOFT_SPI_H_ */
