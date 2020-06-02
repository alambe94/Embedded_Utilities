#include "button.h"

Button_Struct_t Button_A;
Button_Struct_t Button_B;
Button_Struct_t Button_C;
Button_Struct_t Button_D;
Button_Struct_t Button_E;

uint32_t Button_Get_Tick()
{
}

void Button_A_Init()
{
    // enable clock, configure as gpio as input
}
void Button_B_Init()
{
}
void Button_C_Init()
{
}
void Button_D_Init()
{
}
void Button_E_Init()
{
}

uint8_t Button_A_Read()
{
}
uint8_t Button_B_Read()
{
}
uint8_t Button_C_Read()
{
}
uint8_t Button_D_Read()
{
}
uint8_t Button_E_Read()
{
}

/* if Button_Loop() is call in ISR */
/* callback should be as short as possible */
/* printf in callback for demo only */
/* or you can ignore the callback and check manually using Button_Get_Clicked_Count function */
void Button_A_Callback(uint8_t clicked_count)
{
#if 0
    if (clicked_count == 255)
    {
        printf("Button_A long pressed\n");
    }
    {
        printf("Button_A clicked %d times\n", clicked_count);
    }
#endif
}
void Button_B_Callback(uint8_t clicked_count)
{
    printf("Button_B clicked %d times\n", clicked_count);
}
void Button_C_Callback(uint8_t clicked_count)
{
    printf("Button_C clicked %d times\n", clicked_count);
}
void Button_D_Callback(uint8_t clicked_count)
{
    printf("Button_D clicked %d times\n", clicked_count);
}
void Button_E_Callback(uint8_t clicked_count)
{
    printf("Button_E clicked %d times\n", clicked_count);
}

void main()
{
    Button_A.Button_Init = Button_A_Init;
    Button_A.Button_Read = Button_A_Read;
    Button_A.Callback = Button_A_Callback;
    Button_Add(&Button_A);

    Button_B.Button_Init = Button_B_Init;
    Button_B.Button_Read = Button_B_Read;
    Button_B.Callback = Button_B_Callback;
    Button_Add(&Button_B);

    Button_C.Button_Init = Button_C_Init;
    Button_C.Button_Read = Button_C_Read;
    Button_C.Callback = Button_C_Callback;
    Button_Add(&Button_C);

    Button_D.Button_Init = Button_D_Init;
    Button_D.Button_Read = Button_D_Read;
    Button_D.Callback = Button_D_Callback;
    Button_Add(&Button_D);

    Button_E.Button_Init = Button_E_Init;
    Button_E.Button_Read = Button_E_Read;
    Button_E.Callback = Button_E_Callback;
    Button_Add(&Button_E);

    while (1)
    {
        Button_Loop();
#if 1
        // ignore callback manually read click count
        uint8_t clicked_count_a = Button_Get_Clicked_Count(&Button_A);
        if (clicked_count_a)
        {
            if (clicked_count_a == 255)
            {
                printf("Button_A long pressed\n");
            }
            {
                printf("Button_A clicked %d times\n", clicked_count_a);
            }
        }
#endif
    }
}