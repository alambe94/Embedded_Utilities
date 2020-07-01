#include "stdio.h"
#include "stack.h"

Stack_t stack1;
uint8_t Stack1_Buffer[100];

void main()
{
    uint8_t data = 0;

    Stack_Init(&stack1, Stack1_Buffer, sizeof(Stack1_Buffer));

    for (uint16_t i = 0; i < 64; i++)
    {
        Stack_Push(&stack1, i);
    }

    while ((Stack_Pop(&stack1, &data)))
    {
        printf("%d\n", data);
    }
}