/*
 * ring_buffer.c
 *
 *  Created on: 01-Aug-2018
 *      Author: medprime
 */

/**
 *  Change log
 *
 *  V1.1:
 *  1. added full_flag to check for full and empty buffer.
 *  1. added Ring_Buffer_Is_Empty function.
 *  2. changed char data type to uint8_t.
 *  3. modified Ring_Buffer_Put_Char and Ring_Buffer_Get_Char functions to use Full_Flag.
 *  4. Ring_Buffer_Peek_Char bug fixed.
 *
 */


#include "ring_buffer.h"

void Ring_Buffer_Init(Ring_Buffer_t* handle, uint8_t* buffer, uint16_t size)
    {
    handle->Buffer        =  buffer;
    handle->Read_Index    =  0;
    handle->Write_Index   =  0;
    handle->Size          =  size;
    handle->Full_Flag     =  0;
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


uint8_t Ring_Buffer_Peek_Char(Ring_Buffer_t* handle, uint8_t* data,
	uint16_t position)
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


uint8_t Ring_Buffer_Search_Char(Ring_Buffer_t* handle, uint8_t data)
    {

    uint8_t  xreturn = 0;
    uint16_t temp = handle->Read_Index;

    while (temp != handle->Write_Index)
	{

	if (data == handle->Buffer[temp++])
	    {
	    xreturn = 1;
	    break;
	    }

	if(temp == handle->Size)
	    {
	    temp = 0;
	    }

	}

    return xreturn;
    }


uint8_t Ring_Buffer_Is_Full(Ring_Buffer_t* handle)
    {
    return handle->Full_Flag;
    }


uint8_t Ring_Buffer_Is_Empty(Ring_Buffer_t* handle)
    {
    return (handle->Read_Index == handle->Write_Index  && handle->Full_Flag == 0);
    }


void Ring_Buffer_Flush(Ring_Buffer_t* handle)
    {
    handle->Read_Index = handle->Write_Index;
    handle->Full_Flag = 0;
    }


uint16_t Ring_Buffer_Get_Count(Ring_Buffer_t* handle)
    {
    if (handle->Write_Index >= handle->Read_Index)
	{
	return (handle->Write_Index - handle->Read_Index);
	}
    return (handle->Size - (handle->Read_Index - handle->Write_Index ));
    }

