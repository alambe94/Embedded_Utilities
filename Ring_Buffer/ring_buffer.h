#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include "stm32XX_hal.h"

typedef struct Ring_Buffer_t
{
	uint8_t *Buffer;
	uint16_t Write_Index;
	uint16_t Read_Index;
	uint16_t Size;
	uint8_t Full_Flag;
} Ring_Buffer_t;

void Ring_Buffer_Init(Ring_Buffer_t *handle, uint8_t *buffer, uint16_t size);
void Ring_Buffer_Flush(Ring_Buffer_t *handle);
uint8_t Ring_Buffer_Put_Char(Ring_Buffer_t *handle, uint8_t data);
uint8_t Ring_Buffer_Get_Char(Ring_Buffer_t *handle, uint8_t *data);
uint8_t Ring_Buffer_Peek_Char(Ring_Buffer_t *handle, uint8_t *data, uint16_t position);
uint8_t Ring_Buffer_Is_Full(Ring_Buffer_t *handle);
uint8_t Ring_Buffer_Is_Empty(Ring_Buffer_t *handle);
uint16_t Ring_Buffer_Get_Count(Ring_Buffer_t *handle);

#endif /* RING_BUFFER_H_ */
