/*
 * file version V0.0.0
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

/** stack includes */
#include "stack.h"

#define USE_STACK_ASSERT 1

#if (USE_STACK_ASSERT == 1)
#include <stdio.h>
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
    STACK_ASSERT(handle, "handle is NULL");
    STACK_ASSERT(buffer, "buffer is NULL");

    if (handle != NULL && buffer != NULL)
    {
        handle->Buffer = buffer;
        handle->Index = 0;
        handle->Size = size;
    }
}

uint8_t Stack_Get_Count(Stack_t *handle)
{
    STACK_ASSERT(handle, "handle is NULL");
    return handle->Index;
}

uint8_t Stack_Clear(Stack_t *handle)
{
    STACK_ASSERT(handle, "handle is NULL");
    return handle->Index = 0;
}

uint8_t Stack_Is_Full(Stack_t *handle)
{
    STACK_ASSERT(handle, "handle is NULL");
    return handle->Index == handle->Size;
}

uint8_t Stack_Is_Empty(Stack_t *handle)
{
    STACK_ASSERT(handle, "handle is NULL");
    return handle->Index == 0;
}

uint8_t Stack_Push(Stack_t *handle, uint8_t data)
{
    STACK_ASSERT(handle, "handle is NULL");

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
    STACK_ASSERT(handle, "handle is NULL");
    STACK_ASSERT(data, "data is NULL");

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
    STACK_ASSERT(handle, "handle is NULL");
    STACK_ASSERT(data, "data is NULL");

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
