/*
 * file version V1.4
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

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

/** std includes */
#include <stdint.h>

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
