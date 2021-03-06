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

/**
 *
 *  Change log
 *  V1.1:
 *  1. added full_flag to check for full and empty buffer.
 *  1. added Ring_Buffer_Is_Empty function.
 *  2. changed char data type to uint8_t.
 *  3. modified Ring_Buffer_Put_Char and Ring_Buffer_Get_Char functions to use Full_Flag.
 *  4. Ring_Buffer_Peek_Char bug fixed.
 *
 *  V1.2:
 *  1. Ring_Buffer_Search_Char now also return the position at witch char is found.
 *  2. ring buffer for generic data type (othe than uint8_t).
 *
 *  V1.3:
 *  1. ring buffer for generic data type and uint8_t merged.
 *  2. wrapper around generic data type for uint8_t ring buffer.
 *
 *  V1.4:
 *  1. added assersions
 */

/** rin buffer includes */
#include "ring_buffer.h"

#define USE_RB_ASSERT 1

/**
 * @brief assert implemenation, set USE_RB_ASSERT to 1 to enable assert
 */
#if (USE_RB_ASSERT == 1)
#include <stdio.h>
#define RB_ASSERT(expr, msg) ((expr) ? (void)0U : RB_Assert(msg, "ring_buffer.c", __LINE__))
static void RB_Assert(char *msg, char *file, uint32_t line)
{
    printf("%s, assertion failed, file=%s, line=%lu\n", msg, file, line);
}
#else
#define RB_ASSERT(expr, msg) ((void)0U)
#endif

/**
 * @brief initialize new instant of ring buffer
 * @param handle handle of ring buffer to be initialized
 * @param buffer storage area for ring buffer
 * @param element_size size of single element in bytes
 * @param max_elements maximum number of element this handle can store
 */
void Ring_Buffer_Init(Ring_Buffer_t *handle, uint8_t *buffer, uint8_t element_size, uint32_t max_elements)
{
    RB_ASSERT(handle, "handle is NULL");
    RB_ASSERT(buffer, "buffer is NULL");

    handle->Storage = buffer;
    handle->Element_Size = element_size;
    handle->MAX_Elements = max_elements;
    handle->Read_Index = 0;
    handle->Write_Index = 0;
    handle->Full_Flag = 0;
}

/**
 * @brief reset or flush the ring buffer
 * @param handle handle of ring buffer
 */
void Ring_Buffer_Flush(Ring_Buffer_t *handle)
{
    RB_ASSERT(handle, "handle is NULL");

    handle->Read_Index = handle->Write_Index;
    handle->Full_Flag = 0;
}

/**
 * @brief return the number of elements in ring buffer
 * @param handle handle of ring buffer
 * @retval count
 */
uint32_t Ring_Buffer_Get_Count(Ring_Buffer_t *handle)
{
    RB_ASSERT(handle, "handle is NULL");

    uint32_t count = 0;

    if (Ring_Buffer_Is_Full(handle))
    {
        count = handle->MAX_Elements;
    }
    else if (handle->Write_Index >= handle->Read_Index)
    {
        count = handle->Write_Index - handle->Read_Index;
    }
    else
    {
        count = handle->MAX_Elements - (handle->Read_Index - handle->Write_Index);
    }

    return count;
}

/**
 * @brief check if ring buffer is full or not
 * @param handle handle of ring buffer
 * @retval Full_Flag return 1 if full
 */
uint8_t Ring_Buffer_Is_Full(Ring_Buffer_t *handle)
{
    RB_ASSERT(handle, "handle is NULL");

    return handle->Full_Flag;
}

/**
 * @brief check if ring buffer is empty or not
 * @param handle handle of ring buffer
 * @retval return 1 if empty
 */
uint8_t Ring_Buffer_Is_Empty(Ring_Buffer_t *handle)
{
    RB_ASSERT(handle, "handle is NULL");

    return (handle->Read_Index == handle->Write_Index && !Ring_Buffer_Is_Full(handle));
}

/**
 * @brief write single element in ring buffer
 * @param handle handle of ring buffer
 * @param data pointer to element
 * @retval return 1 if write is successful
 */
uint8_t Ring_Buffer_Put(Ring_Buffer_t *handle, void *data)
{
    RB_ASSERT(handle, "handle is NULL");
    RB_ASSERT(data, "data is NULL");

    uint8_t xreturn = 0;
    uint8_t *ptr = (uint8_t *)data;

    if (Ring_Buffer_Is_Full(handle))
    {
        //over flow return 0 ?
        RB_ASSERT(0, "Ring buffer is full");
    }
    else
    {
        xreturn = 1;

        for (uint8_t i = 0; i < handle->Element_Size; i++)
        {
            handle->Storage[handle->Write_Index * handle->Element_Size + i] = *ptr++;
        }

        handle->Write_Index++;

        if (handle->Write_Index == handle->MAX_Elements)
        {
            handle->Write_Index = 0;
        }

        if (handle->Write_Index == handle->Read_Index)
        {
            handle->Full_Flag = 1;
        }
    }

    return xreturn;
}

/**
 * @brief retrieve single element from ring buffer
 * @param handle handle of ring buffer
 * @param data where to retrieve
 * @retval return 1 if write is successful
 */
uint8_t Ring_Buffer_Get(Ring_Buffer_t *handle, void *data)
{
    RB_ASSERT(handle, "handle is NULL");
    RB_ASSERT(data, "data is NULL");

    uint8_t xreturn = 0;
    uint8_t *ptr = (uint8_t *)data;

    if (!Ring_Buffer_Is_Empty(handle))
    {
        xreturn = 1;

        for (uint8_t i = 0; i < handle->Element_Size; i++)
        {
            *ptr++ = handle->Storage[handle->Read_Index * handle->Element_Size + i];
        }

        handle->Read_Index++;

        handle->Full_Flag = 0;

        if (handle->Read_Index == handle->MAX_Elements)
        {
            handle->Read_Index = 0;
        }
    }

    return xreturn;
}

/**
 * @brief peek element at given position
 * @param handle handle of ring buffer
 * @param data where to retrieve peeked element
 * @param position where to peek
 * @retval return 1 if successful
 */
uint8_t Ring_Buffer_Peek(Ring_Buffer_t *handle, void *data, uint32_t position)
{
    RB_ASSERT(handle, "handle is NULL");
    RB_ASSERT(data, "data is NULL");

    uint8_t xreturn = 0;
    uint8_t *ptr = (uint8_t *)data;

    if (position < Ring_Buffer_Get_Count(handle))
    {
        xreturn = 1;

        if (position + handle->Read_Index < handle->MAX_Elements)
        {
            position = handle->Read_Index + position;
        }
        else
        {
            position = (position + handle->Read_Index) - handle->MAX_Elements;
        }

        for (uint8_t i = 0; i < handle->Element_Size; i++)
        {
            *ptr++ = handle->Storage[position * handle->Element_Size + i];
        }
    }

    return xreturn;
}

/**
 * @brief search if given element is present in ring buffer
 * @param handle handle of ring buffer
 * @param data element to be searched
 * @param position where element is found
 * @retval return 1 if element is found
 */
uint8_t Ring_Buffer_Search(Ring_Buffer_t *handle, void *data, uint32_t *position)
{
    RB_ASSERT(handle, "handle is NULL");
    RB_ASSERT(data, "data is NULL");
    RB_ASSERT(position, "position is NULL");

    uint8_t xreturn = 1;
    uint8_t *ptr = (uint8_t *)data;
    uint32_t temp = handle->Read_Index;
    uint32_t count = Ring_Buffer_Get_Count(handle);

    while (count--)
    {
        xreturn = 1;

        for (uint8_t i = 0; i < handle->Element_Size; i++)
        {
            if (ptr[i] != handle->Storage[temp * handle->Element_Size + i])
            {
                xreturn = 0;
                break;
            }
        }

        if (xreturn)
        {
            *position = temp;
            break;
        }

        temp++;

        if (temp == handle->MAX_Elements)
        {
            temp = 0;
        }
    }

    return xreturn;
}

/**
 * @brief wrapper function for single byte element write
 * @param handle handle of ring buffer
 * @param data byte to be written
 * @retval return 1 if write is successful
 */
uint8_t Ring_Buffer_Put_Char(Ring_Buffer_t *handle, uint8_t data)
{
    RB_ASSERT(handle, "handle is NULL");

    return Ring_Buffer_Put(handle, &data);
}

/**
 * @brief wrapper function for single byte element read
 * @param handle handle of ring buffer
 * @param data where byte is retrieved
 * @retval return 1 if read is successful
 */
uint8_t Ring_Buffer_Get_Char(Ring_Buffer_t *handle, uint8_t *data)
{
    RB_ASSERT(handle, "handle is NULL");
    RB_ASSERT(data, "data is NULL");

    return Ring_Buffer_Get(handle, data);
}

/**
 * @brief wrapper function for single byte element peek
 * @param handle handle of ring buffer
 * @param data where byte is retrieved
 * @retval return 1 if successful
 */
uint8_t Ring_Buffer_Peek_Char(Ring_Buffer_t *handle, uint8_t *data, uint32_t position)
{
    RB_ASSERT(handle, "handle is NULL");
    RB_ASSERT(data, "data is NULL");

    return Ring_Buffer_Peek(handle, data, position);
}

/**
 * @brief wrapper function for single byte element search
 * @param handle handle of ring buffer
 * @param data byte to be search
 * @param position where byte is found
 * @retval return 1 if byte is found
 */
uint8_t Ring_Buffer_Search_Char(Ring_Buffer_t *handle, uint8_t data, uint32_t *position)
{
    RB_ASSERT(handle, "handle is NULL");
    RB_ASSERT(position, "position is NULL");

    return Ring_Buffer_Search(handle, &data, position);
}
