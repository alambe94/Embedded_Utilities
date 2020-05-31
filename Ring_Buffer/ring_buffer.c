/**
 *  V1.2
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
 *  1.  Ring_Buffer_Search_Char now also return the position at witch char found.
 *  2.  ring buffer for generic data type (othe than uint8_t).
 */

#include "ring_buffer.h"
#include "memory.h"

void Ring_Buffer_Init(Ring_Buffer_t *handle, uint8_t *buffer, uint16_t size)
{
    handle->Buffer = buffer;
    handle->Read_Index = 0;
    handle->Write_Index = 0;
    handle->Size = size;
    handle->Full_Flag = 0;
}

uint8_t Ring_Buffer_Put_Char(Ring_Buffer_t *handle, uint8_t data)
{
    uint8_t xreturn = 0;

    if (Ring_Buffer_Is_Full(handle))
    {
        //over flow return 0 ?
    }
    else
    {
        xreturn = 1;

        handle->Buffer[handle->Write_Index] = data;

        handle->Write_Index++;

        if (handle->Write_Index == handle->Size)
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

uint8_t Ring_Buffer_Get_Char(Ring_Buffer_t *handle, uint8_t *data)
{
    uint8_t xreturn = 0;

    if (Ring_Buffer_Is_Empty(handle))
    {
        //*data = 0;
    }
    else
    {
        xreturn = 1;

        *data = handle->Buffer[handle->Read_Index];

        handle->Read_Index++;

        handle->Full_Flag = 0;

        if (handle->Read_Index == handle->Size)
        {
            handle->Read_Index = 0;
        }
    }

    return xreturn;
}

uint8_t Ring_Buffer_Peek_Char(Ring_Buffer_t *handle, uint8_t *data, uint16_t position)
{
    uint8_t xreturn = 0;

    if (position < Ring_Buffer_Get_Count(handle))
    {
        xreturn = 1;

        if (position + handle->Read_Index < handle->Size)
        {
            position = handle->Read_Index + position;
        }
        else
        {
            position = (position + handle->Read_Index) - handle->Size;
        }

        *data = handle->Buffer[position];
    }
    else
    {
        *data = 0;
    }

    return xreturn;
}

uint8_t Ring_Buffer_Search_Char(Ring_Buffer_t *handle, uint8_t data, uint16_t *posotion)
{
    uint8_t xreturn = 0;
    uint16_t temp = handle->Read_Index;

    while (temp != handle->Write_Index)
    {
        if (data == handle->Buffer[temp++])
        {
            xreturn = 1;
            *posotion = temp - 1;
            break;
        }

        if (temp == handle->Size)
        {
            temp = 0;
        }
    }

    return xreturn;
}

uint8_t Ring_Buffer_Is_Full(Ring_Buffer_t *handle)
{
    return handle->Full_Flag;
}

uint8_t Ring_Buffer_Is_Empty(Ring_Buffer_t *handle)
{
    return (handle->Read_Index == handle->Write_Index && handle->Full_Flag == 0);
}

void Ring_Buffer_Flush(Ring_Buffer_t *handle)
{
    handle->Read_Index = handle->Write_Index;
    handle->Full_Flag = 0;
}

uint16_t Ring_Buffer_Get_Count(Ring_Buffer_t *handle)
{
    if (handle->Write_Index >= handle->Read_Index)
    {
        return (handle->Write_Index - handle->Read_Index);
    }
    return (handle->Size - (handle->Read_Index - handle->Write_Index));
}

void RB_Generic_Init(RB_Generic_t *handle, uint8_t *buffer, uint8_t element_size, size_t max_elements)
{
    handle->Storage = buffer;
    handle->Element_Size = element_size;
    handle->MAX_Elements = max_elements;
    handle->Read_Index = 0;
    handle->Write_Index = 0;
    handle->Full_Flag = 0;
}

void RB_Generic_Flush(RB_Generic_t *handle)
{
    handle->Read_Index = handle->Write_Index;
    handle->Full_Flag = 0;
}

size_t RB_Generic_Get_Count(RB_Generic_t *handle)
{
    if (handle->Write_Index >= handle->Read_Index)
    {
        return (handle->Write_Index - handle->Read_Index);
    }
    return (handle->MAX_Elements - (handle->Read_Index - handle->Write_Index));
}

uint8_t RB_Generic_Is_Full(RB_Generic_t *handle)
{
    return handle->Full_Flag;
}

uint8_t RB_Generic_Is_Empty(RB_Generic_t *handle)
{
    return (handle->Read_Index == handle->Write_Index && handle->Full_Flag == 0);
}

uint8_t RB_Generic_Put(RB_Generic_t *handle, void *data)
{
    uint8_t xreturn = 0;
    uint8_t *ptr = (uint8_t *)data;

    if (RB_Generic_Is_Full(handle))
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

uint8_t RB_Generic_Get(RB_Generic_t *handle, void *data)
{
    uint8_t xreturn = 0;
    uint8_t *ptr = (uint8_t *)data;

    if (RB_Generic_Is_Empty(handle))
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

uint8_t RB_Generic_Peek(RB_Generic_t *handle, void *data, size_t position)
{
    uint8_t xreturn = 0;
    uint8_t *ptr = (uint8_t *)data;

    if (position < RB_Generic_Get_Count(handle))
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

uint8_t RB_Generic_Search(RB_Generic_t *handle, void *data, size_t *posotion)
{
    uint8_t xreturn = 0;
    uint8_t *ptr = (uint8_t *)data;
    size_t temp = handle->Read_Index;

    while (temp != handle->Write_Index)
    {
        if (memcmp(ptr++, &handle->Storage[temp * handle->Element_Size], handle->Element_Size) == 0)
        {
            xreturn = 1;
            *posotion = temp - 1;
            break;
        }

        if (temp == handle->MAX_Elements)
        {
            temp = 0;
        }
    }

    return xreturn;
}