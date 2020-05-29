#ifndef STACK_H_
#define STACK_H_

#include "stm32XX_hal.h"

typedef struct Stack_t
{
    uint8_t *Buffer;
    uint16_t Count;
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