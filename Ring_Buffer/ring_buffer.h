#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include "stdint.h"

typedef struct Ring_Buffer_t
{
	uint8_t *Buffer;
	uint8_t Full_Flag;
	uint16_t Write_Index;
	uint16_t Read_Index;
	uint16_t Size;

} Ring_Buffer_t;

void Ring_Buffer_Init(Ring_Buffer_t *handle, uint8_t *buffer, uint16_t size);
void Ring_Buffer_Flush(Ring_Buffer_t *handle);
uint16_t Ring_Buffer_Get_Count(Ring_Buffer_t *handle);
uint8_t Ring_Buffer_Is_Full(Ring_Buffer_t *handle);
uint8_t Ring_Buffer_Is_Empty(Ring_Buffer_t *handle);
uint8_t Ring_Buffer_Put_Char(Ring_Buffer_t *handle, uint8_t data);
uint8_t Ring_Buffer_Get_Char(Ring_Buffer_t *handle, uint8_t *data);
uint8_t Ring_Buffer_Peek_Char(Ring_Buffer_t *handle, uint8_t *data, uint16_t position);
uint8_t Ring_Buffer_Search_Char(Ring_Buffer_t *handle, uint8_t data, uint16_t *posotion);





typedef struct RB_Generic_t
{
	uint8_t *Storage;
	uint8_t Full_Flag;
	uint8_t Element_Size;
	size_t MAX_Elements;
	size_t Write_Index;
	size_t Read_Index;

} RB_Generic_t;

void RB_Generic_Init(RB_Generic_t *handle, uint8_t *buffer, uint8_t element_size, size_t max_elements);
void RB_Generic_Flush(RB_Generic_t *handle);
size_t RB_Generic_Get_Count(RB_Generic_t *handle);
uint8_t RB_Generic_Is_Full(RB_Generic_t *handle);
uint8_t RB_Generic_Is_Empty(RB_Generic_t *handle);
uint8_t RB_Generic_Put(RB_Generic_t *handle, void *buffer);
uint8_t RB_Generic_Get(RB_Generic_t *handle, void *buffer);
uint8_t RB_Generic_Peek(RB_Generic_t *handle, void *data, size_t position);
uint8_t RB_Generic_Search(RB_Generic_t *handle, void *data, size_t *posotion);

#endif /* RING_BUFFER_H_ */
