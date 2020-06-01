#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include "stdint.h"

typedef struct Ring_Buffer_t
{
	uint8_t *Storage;
	uint8_t Full_Flag;
	uint8_t Element_Size;
	uint32_t MAX_Elements;
	uint32_t Write_Index;
	uint32_t Read_Index;

} Ring_Buffer_t;

void Ring_Buffer_Init(Ring_Buffer_t *handle, uint8_t *buffer, uint8_t element_size, uint32_t max_elements);
void Ring_Buffer_Flush(Ring_Buffer_t *handle);
uint32_t Ring_Buffer_Get_Count(Ring_Buffer_t *handle);
uint8_t Ring_Buffer_Is_Full(Ring_Buffer_t *handle);
uint8_t Ring_Buffer_Is_Empty(Ring_Buffer_t *handle);
uint8_t Ring_Buffer_Put(Ring_Buffer_t *handle, void *buffer);
uint8_t Ring_Buffer_Get(Ring_Buffer_t *handle, void *buffer);
uint8_t Ring_Buffer_Peek(Ring_Buffer_t *handle, void *data, uint32_t position);
uint8_t Ring_Buffer_Search(Ring_Buffer_t *handle, void *data, uint32_t *position);

uint8_t Ring_Buffer_Put_Char(Ring_Buffer_t *handle, uint8_t data);
uint8_t Ring_Buffer_Get_Char(Ring_Buffer_t *handle, uint8_t *data);
uint8_t Ring_Buffer_Peek_Char(Ring_Buffer_t *handle, uint8_t *data, uint32_t position);
uint8_t Ring_Buffer_Search_Char(Ring_Buffer_t *handle, uint8_t data, uint32_t *position);

#endif /* RING_BUFFER_H_ */
