##[STM32 Example](https://github.com/alambe94/STM32_Button.git)

This is simple Button Library to read button event such as single, double , triple ... up to 254 clicked!!! and long pressed event with optional callback.

## How to use
### External functions dependencies
1. Button_Get_Tick(), used for timing purpose. simply return millis or tick elapsed.
```
uint32_t Button_Get_Tick()
{
    return HAL_GetTick();
}
```
2. GPIO Init, enable clock, configure as gpio as input.
3. GPIO Pin Read, returns pin status.
3. (Optional)Event callback. Called after an event has ocurred.

### Code
1. Create button handles
```
Button_Struct_t Button_A;
Button_Struct_t Button_B;
Button_Struct_t Button_C;
Button_Struct_t Button_D;
Button_Struct_t Button_E;
```
2. Create GPIO Init function
```
void Button_A_Init()
{
    // enable clock, configure as gpio as input
}
void Button_B_Init()
{
   // enable clock, configure as gpio as input
}
```
3. Provide GPIO Pin Read functions
```
// Button logic high
uint8_t Button_A_Read()
{
    return HAL_GPIO_ReadPin(A_GPIO_Port, A_Pin);
}
```
```
// Button logic low
uint8_t Button_B_Read()
{
    return HAL_GPIO_ReadPin(B_GPIO_Port, B_Pin) ? 0 : 1;
}
```
4. Create optional callbacks
```
void Button_A_Callback(uint8_t clicked_count)
{
    if (clicked_count == 255)
    {
        printf("Button_A long pressed\n");
    }
    else if (clicked_count == 1)
    {
    }
    else if (clicked_count == 2)
    {
    }
    else if (clicked_count == 3)
    {
    }
}
void Button_B_Callback(uint8_t clicked_count)
{
    printf("Button_B clicked %d times\n", clicked_count);
}
```
5. Add handles to list of registered buttons
```
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
    Button_C.Callback = NULL; /** without callback */
    Button_Add(&Button_C);
    .
    .
    .
```
6. Call Button_Loop() frequently. In main loop or in Timer ISR. Note, If Called in Timer ISR and callback is defined, then callback should be as short as possible.
```
while (1)
    {
        Button_Loop();
    }
```
```
Systick_ISR()
    {
        Button_Loop();
    }
```
7. If callback is defined it will be called when any event happens. If not we can read manually
```
    clicked_count_a = Button_Get_Clicked_Count(&Button_A);
    clicked_count_b = Button_Get_Clicked_Count(&Button_B);
    clicked_count_c = Button_Get_Clicked_Count(&Button_C);

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
```
