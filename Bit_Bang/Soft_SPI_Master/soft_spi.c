#include "soft_spi.h"

void Soft_Uart_Init(Soft_SPI_t *spi_handle)
{
	/**** GPIO pin configured in cube *********/
}

void Soft_SPI_Send_Receive_Byte(Soft_SPI_t *spi_handle, uint8_t tx_data, uint8_t *rx_data)
{

	uint8_t rx_data_temp = 0;

	for (uint8_t i = 0; i < 8; i++)
	{

		/* accumulate rx data, left shift*/
		rx_data_temp <<= 1;

		if (tx_data & 0x80)
		{

			HAL_GPIO_WritePin(spi_handle->GPIO_MISO_Port,
							  spi_handle->GPIO_MOSI_Pin, GPIO_PIN_SET);
		}
		else
		{

			HAL_GPIO_WritePin(spi_handle->GPIO_MISO_Port,
							  spi_handle->GPIO_MOSI_Pin, GPIO_PIN_RESET);
		}

		HAL_GPIO_WritePin(spi_handle->GPIO_SCK_Port,
						  spi_handle->GPIO_SCK_Pin, GPIO_PIN_SET);

		if (HAL_GPIO_ReadPin(spi_handle->GPIO_MISO_Port, spi_handle->GPIO_MISO_Pin))
		{
			rx_data_temp |= 0x01;
		}

		HAL_GPIO_WritePin(spi_handle->GPIO_SCK_Port,
						  spi_handle->GPIO_SCK_Pin, GPIO_PIN_RESET);

		/*left shift to transmit next bit*/
		tx_data <<= 1;
	}

	*rx_data = rx_data_temp;
}

void Soft_SPI_Send_Bytes(Soft_SPI_t *spi_handle, uint8_t *buffer, uint16_t len)
{

	uint8_t rx_data;
	uint8_t tx_data = 0;

	while (len--)
	{
		tx_data = *buffer++;
		Soft_SPI_Send_Receive_Byte(spi_handle, tx_data, &rx_data);
	}
}

void Soft_SPI_Receive_Bytes(Soft_SPI_t *spi_handle, uint8_t *rx_buffer, uint16_t len)
{

	uint8_t tx_data = 0XFF; // dummy byte
	uint8_t rx_data;
	;

	while (len--)
	{
		Soft_SPI_Send_Receive_Byte(spi_handle, tx_data, &rx_data);
		*rx_buffer = rx_data;
		rx_buffer++;
	}
}
