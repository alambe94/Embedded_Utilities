##[STM32 Example](https://github.com/alambe94/STM32_Encoder.git)

Encoder Library to read general purpose encoders used in menu navigation. (not used for motor!!!)

## How to use
### External functions dependencies
1. Encoder_Get_Tick(), used for timing purpose. simply return millis or tick elapsed.
```
uint32_t Encoder_Get_Tick()
{
    return HAL_GetTick();
}
```
2. GPIO Init, enable clock, configure as gpio as input.
3. GPIO Pin Read, returns pin status.

### Code
1. Create encoder handles
```
Encoder_Struct_t Encoder_X;
Encoder_Struct_t Encoder_Y;
Encoder_Struct_t Encoder_Z;
```
2. Create GPIO Init function
```
void Encoder_X_Init()
{
    // enable clock, configure as gpio as input
}
void Encoder_Y_Init()
{
}
void Encoder_Z_Init()
{
}
```
3. Provide GPIO Pin Read functions
```
uint8_t Encoder_X_Read_Pin_A()
{
    return HAL_GPIO_ReadPin(ENC_X_A_GPIO_Port, ENC_X_A_Pin);
}
uint8_t Encoder_X_Read_Pin_B()
{
    return HAL_GPIO_ReadPin(ENC_X_B_GPIO_Port, ENC_X_B_Pin);
}

uint8_t Encoder_Y_Read_Pin_A()
{
    return HAL_GPIO_ReadPin(ENC_Y_A_GPIO_Port, ENC_Y_A_Pin);
}
uint8_t Encoder_Y_Read_Pin_B()
{
    return HAL_GPIO_ReadPin(ENC_Y_A_GPIO_Port, ENC_Y_A_Pin);
}
```
4. Add handles to list of registered encoders
```
    Encoder_X.Encoder_Init = Encoder_X_Init;
    Encoder_X.Encoder_Read_Pin_A = Encoder_X_Read_Pin_A;
    Encoder_X.Encoder_Read_Pin_B = Encoder_X_Read_Pin_B;
    Encoder_Add(&Encoder_X);

    Encoder_Y.Encoder_Init = Encoder_Y_Init;
    Encoder_Y.Encoder_Read_Pin_A = Encoder_Y_Read_Pin_A;
    Encoder_Y.Encoder_Read_Pin_B = Encoder_Y_Read_Pin_B;
    Encoder_Add(&Encoder_Y);
    .
    .
    .
```
5. Call Encoder_Loop() frequently. In main loop or in Timer ISR.
```
while (1)
    {
        Encoder_Loop();
    }
```
```
Systick_ISR()
    {
        Encoder_Loop();
    }
```
6. Read encoders
```
    count_x = Encoder_Get_Count(&Encoder_X);
    count_y = Encoder_Get_Count(&Encoder_Y);
    count_z = Encoder_Get_Count(&Encoder_Z);

    if (count_x)
    {
        count_x_accumulated += count_x;
        printf("X count = %d\n", count_x_accumulated);
    }
    if (count_y)
    {
        count_y_accumulated += count_y;
        printf("Y count = %d\n", count_y_accumulated);
    }
    if (count_z)
    {
        count_z_accumulated += count_z;
        printf("Z count = %d\n", count_z_accumulated);
    }
```