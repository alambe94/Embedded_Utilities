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

#ifndef STACK_H_
#define STACK_H_

#include "stdint.h"

#define USE_STACK_ASSERT 1

typedef struct Stack_t
{
    uint8_t *Buffer;
    uint16_t Index;
    uint16_t Size;

} Stack_t;

void Stack_Init(Stack_t *handle, uint8_t *buffer, uint16_t size);
uint8_t Stack_Get_Count();
uint8_t Stack_Clear();
uint8_t Stack_Is_Full(Stack_t *handle);
uint8_t Stack_Is_Empty(Stack_t *stack);
uint8_t Stack_Push(Stack_t *handle, uint8_t data);
uint8_t Stack_Pop(Stack_t *handle, uint8_t *data);
uint8_t Stack_Peek(Stack_t *handle, uint8_t *data);

#endif /* STACK_H_ */