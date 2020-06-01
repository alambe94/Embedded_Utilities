/**
 *  V1.3
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
 */

#include "ring_buffer.h"
#include "memory.h"

void Ring_Buffer_Init(Ring_Buffer_t *handle, uint8_t *buffer, uint8_t element_size, uint32_t max_elements)
{
    handle->Storage = buffer;
    handle->Element_Size = element_size;
    handle->MAX_Elements = max_elements;
    handle->Read_Index = 0;
    handle->Write_Index = 0;
    handle->Full_Flag = 0;
}

void Ring_Buffer_Flush(Ring_Buffer_t *handle)
{
    handle->Read_Index = handle->Write_Index;
    handle->Full_Flag = 0;
}

uint32_t Ring_Buffer_Get_Count(Ring_Buffer_t *handle)
{
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

uint8_t Ring_Buffer_Is_Full(Ring_Buffer_t *handle)
{
    return handle->Full_Flag;
}

uint8_t Ring_Buffer_Is_Empty(Ring_Buffer_t *handle)
{
    return (handle->Read_Index == handle->Write_Index && handle->Full_Flag == 0);
}

uint8_t Ring_Buffer_Put(Ring_Buffer_t *handle, void *data)
{
    uint8_t xreturn = 0;
    uint8_t *ptr = (uint8_t *)data;

    if (Ring_Buffer_Is_Full(handle))
    {
        //over flow return 0 ?
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

uint8_t Ring_Buffer_Get(Ring_Buffer_t *handle, void *data)
{
    uint8_t xreturn = 0;
    uint8_t *ptr = (uint8_t *)data;

    if (Ring_Buffer_Is_Empty(handle))
    {
        //*data = 0;
    }
    else
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

uint8_t Ring_Buffer_Peek(Ring_Buffer_t *handle, void *data, uint32_t position)
{
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

uint8_t Ring_Buffer_Search(Ring_Buffer_t *handle, void *data, uint32_t *position)
{
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

uint8_t Ring_Buffer_Put_Char(Ring_Buffer_t *handle, uint8_t data)
{
    return Ring_Buffer_Put(handle, &data);
}

uint8_t Ring_Buffer_Get_Char(Ring_Buffer_t *handle, uint8_t *data)
{
    return Ring_Buffer_Get(handle, data);
}

uint8_t Ring_Buffer_Peek_Char(Ring_Buffer_t *handle, uint8_t *data, uint32_t position)
{
    return Ring_Buffer_Peek(handle, data, position);
}

uint8_t Ring_Buffer_Search_Char(Ring_Buffer_t *handle, uint8_t data, uint32_t *position)
{
    return Ring_Buffer_Search(handle, &data, position);
}