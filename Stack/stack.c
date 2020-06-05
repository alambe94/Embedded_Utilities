#include "stack.h"

#if (USE_STACK_ASSERT == 1)
#include "stdio.h"
#define STACK_ASSERT(expr, msg) ((expr) ? (void)0U : Stack_Assert(msg, "stack.c", __LINE__))
static void Stack_Assert(char *msg, char *file, uint32_t line)
{
    printf("%s, assertion failed, file=%s, line=%lu\n", msg, file, line);
}
#else
#define STACK_ASSERT(expr, msg) ((void)0U)
#endif

void Stack_Init(Stack_t *handle, uint8_t *buffer, uint16_t size)
{
    STACK_ASSERT(handle, "NULL Passed");
    STACK_ASSERT(buffer, "NULL Passed");

    if (handle != NULL && buffer != NULL)
    {
        handle->Buffer = buffer;
        handle->Index = 0;
        handle->Size = size;
    }
}

uint8_t Stack_Get_Count(Stack_t *handle)
{
    STACK_ASSERT(handle, "NULL Passed");
    return handle->Index;
}

uint8_t Stack_Clear(Stack_t *handle)
{
    STACK_ASSERT(handle, "NULL Passed");
    return handle->Index = 0;
}

uint8_t Stack_Is_Full(Stack_t *handle)
{
    STACK_ASSERT(handle, "NULL Passed");
    return handle->Index == handle->Size;
}

uint8_t Stack_Is_Empty(Stack_t *handle)
{
    STACK_ASSERT(handle, "NULL Passed");
    return handle->Index == 0;
}

uint8_t Stack_Push(Stack_t *handle, uint8_t data)
{
    STACK_ASSERT(handle, "NULL Passed");

    uint8_t xreturn = 0;

    if (Stack_Is_Full(handle))
    {
        //over flow return 0 ?
        STACK_ASSERT(handle, "Stack is full");
    }
    else
    {
        xreturn = 1;

        handle->Buffer[handle->Index] = data;

        handle->Index++;
    }

    return xreturn;
}

uint8_t Stack_Pop(Stack_t *handle, uint8_t *data)
{
    STACK_ASSERT(handle, "NULL Passed");
    STACK_ASSERT(data, "NULL Passed");

    uint8_t xreturn = 0;

    if (Stack_Is_Empty(handle))
    {
        //over flow return 0 ?
    }
    else
    {
        xreturn = 1;

        *data = handle->Buffer[handle->Index - 1];

        handle->Index--;
    }

    return xreturn;
}

uint8_t Stack_Peek(Stack_t *handle, uint8_t *data)
{
    STACK_ASSERT(handle, "NULL Passed");
    STACK_ASSERT(data, "NULL Passed");

    uint8_t xreturn = 0;

    if (Stack_Is_Empty(handle))
    {
        //over flow return 0 ?
    }
    else
    {
        xreturn = 1;

        *data = handle->Buffer[handle->Index - 1];
    }

    return xreturn;
}
