#include "stack.h"

void Stack_Init(Stack_t *handle, uint8_t *buffer, uint16_t size)
{
    handle->Buffer = buffer;
    handle->Count = 0;
    handle->Size = size;
}

uint8_t Stack_Get_Count(Stack_t *handle)
{
    return handle->Count;
}

uint8_t Stack_Clear(Stack_t *handle)
{
    return handle->Count = 0;
}

uint8_t Stack_Is_Full(Stack_t *handle)
{
    return handle->Count == handle->Size;
}

uint8_t Stack_Is_Empty(Stack_t *handle)
{
    return handle->Count == 0;
}

uint8_t Stack_Push(Stack_t *handle, uint8_t data)
{
    uint8_t xreturn = 0;

    if (Stack_Is_Full(handle))
    {
        //over flow return 0 ?
    }
    else
    {
        xreturn = 1;

        handle->Buffer[handle->Count] = data;

        handle->Count++;
    }

    return xreturn;
}

uint8_t Stack_Pop(Stack_t *handle, uint8_t *data)
{
    uint8_t xreturn = 0;

    if (Stack_Is_Empty(handle))
    {
        //over flow return 0 ?
    }
    else
    {
        xreturn = 1;

        *data = handle->Buffer[handle->Count - 1];

        handle->Count--;
    }

    return xreturn;
}

uint8_t Stack_Peek(Stack_t *handle, uint8_t *data)
{
    uint8_t xreturn = 0;

    if (Stack_Is_Empty(handle))
    {
        //over flow return 0 ?
    }
    else
    {
        xreturn = 1;

        *data = handle->Buffer[handle->Count - 1];
    }

    return xreturn;
}
