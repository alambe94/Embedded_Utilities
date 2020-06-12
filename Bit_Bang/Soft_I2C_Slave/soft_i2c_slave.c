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

#include "soft_i2c_slave.h"

#include "stm32f4xx_hal.h"

#define SOFT_I2C_SLAVE_SCL_PIN GPIO_PIN_8
#define SOFT_I2C_SLAVE_SCL_PORT GPIOC

#define SOFT_I2C_SLAVE_SDA_PIN GPIO_PIN_6
#define SOFT_I2C_SLAVE_SDA_PORT GPIOC

#define SOFT_I2C_SLAVE_ADDRESS 0x50

struct Soft_I2C_Slave_t Soft_I2C_Slave;

static void Soft_I2C_Slave_SCL_Enable_INT()
{
    SET_BIT(EXTI->IMR, SOFT_I2C_SLAVE_SCL_PIN);
}

static void Soft_I2C_Slave_SCL_Disable_INT()
{
    CLEAR_BIT(EXTI->IMR, SOFT_I2C_SLAVE_SCL_PIN);
}

void Soft_I2C_Slave_Init(void (*Event_Callback)(Soft_I2C_Slave_Event_t event))
{
    Soft_I2C_Slave.Own_Address = SOFT_I2C_SLAVE_ADDRESS << 1;
    Soft_I2C_Slave.callback = Event_Callback;

    Soft_I2C_Slave.state = Slave_Detect_Start;
    Soft_I2C_Slave.Active_Flag = 0;
    Soft_I2C_Slave.Bit_Count = 0;
    Soft_I2C_Slave.Current_Byte = 0;
    Soft_I2C_Slave.Buffer_Index = 0;
    Soft_I2C_Slave.Byte_Count = 0;

    /* GPIO Ports Clock Enable */
    //__HAL_RCC_GPIOA_CLK_ENABLE();
    //__HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    //__HAL_RCC_GPIOD_CLK_ENABLE();
    //__HAL_RCC_GPIOE_CLK_ENABLE();
    //__HAL_RCC_GPIOF_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    HAL_GPIO_WritePin(SOFT_I2C_SLAVE_SCL_PORT, SOFT_I2C_SLAVE_SCL_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SOFT_I2C_SLAVE_SDA_PORT, SOFT_I2C_SLAVE_SDA_PIN, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = SOFT_I2C_SLAVE_SCL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(SOFT_I2C_SLAVE_SCL_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SOFT_I2C_SLAVE_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(SOFT_I2C_SLAVE_SDA_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    Soft_I2C_Slave_SCL_Disable_INT();
}

static void Soft_I2C_Slave_SDA_High()
{
    HAL_GPIO_WritePin(SOFT_I2C_SLAVE_SDA_PORT, SOFT_I2C_SLAVE_SDA_PIN, GPIO_PIN_SET);
}

static void Soft_I2C_Slave_SDA_Low()
{
    HAL_GPIO_WritePin(SOFT_I2C_SLAVE_SDA_PORT, SOFT_I2C_SLAVE_SDA_PIN, GPIO_PIN_RESET);
}

static uint8_t Soft_I2C_Slave_SDA_Read()
{
    return HAL_GPIO_ReadPin(SOFT_I2C_SLAVE_SDA_PORT, SOFT_I2C_SLAVE_SDA_PIN);
}

static uint8_t Soft_I2C_Slave_ACK_Read()
{
    return HAL_GPIO_ReadPin(SOFT_I2C_SLAVE_SDA_PORT, SOFT_I2C_SLAVE_SDA_PIN) ? 0 : 1;
}

static uint8_t Soft_I2C_Slave_SCL_Read()
{
    return HAL_GPIO_ReadPin(SOFT_I2C_SLAVE_SCL_PORT, SOFT_I2C_SLAVE_SCL_PIN);
}

static void Soft_I2C_Slave_State_Machine_SDA_ISR()
{
    if (Soft_I2C_Slave_SCL_Read())
    {
        /*start detected*/
        if (!Soft_I2C_Slave_SDA_Read())
        {
            Soft_I2C_Slave_SCL_Enable_INT();
            Soft_I2C_Slave.Active_Flag = 1;
            Soft_I2C_Slave.state = Slave_Address_In;

            if (Soft_I2C_Slave.callback != NULL)
            {
                Soft_I2C_Slave.callback(Slave_Start_Detected);
            }
        }
        else
        {
            if (Soft_I2C_Slave.callback != NULL)
            {
                Soft_I2C_Slave.callback(Slave_Stop_Detected);
            }
            Soft_I2C_Slave.state = Slave_Detect_Start;
            Soft_I2C_Slave.Active_Flag = 0;
            Soft_I2C_Slave.Bit_Count = 0;
            Soft_I2C_Slave.Current_Byte = 0;
            Soft_I2C_Slave.Buffer_Index = 0;
            Soft_I2C_Slave.Byte_Count = 0;
            Soft_I2C_Slave_SCL_Disable_INT();
        }
    }
}

static void Soft_I2C_Slave_State_Machine_SCL_ISR()
{
    /*rising edge*/
    if (Soft_I2C_Slave_SCL_Read())
    {
        if (Soft_I2C_Slave.state == Slave_Detect_Start || Soft_I2C_Slave.state == Slave_Detect_Stop)
        {
            Soft_I2C_Slave_SDA_High();
        }
        else if (Soft_I2C_Slave.state == Slave_Address_In)
        {
            Soft_I2C_Slave.Current_Byte <<= 1;

            if (Soft_I2C_Slave_SDA_Read())
            {
                Soft_I2C_Slave.Current_Byte |= 0x01;
            }

            Soft_I2C_Slave.Bit_Count++;

            if (Soft_I2C_Slave.Bit_Count == 8)
            {
                Soft_I2C_Slave.Bit_Count = 0;
                Soft_I2C_Slave.Received_Address = Soft_I2C_Slave.Current_Byte;
                Soft_I2C_Slave.Current_Byte = 0;

                if (Soft_I2C_Slave.Received_Address == Soft_I2C_Slave.Own_Address)
                {
                    if (Soft_I2C_Slave.callback != NULL)
                    {
                        Soft_I2C_Slave.callback(Slave_Write_Request);
                    }
                }
                else if (Soft_I2C_Slave.Received_Address == (Soft_I2C_Slave.Own_Address + 1))
                {
                    if (Soft_I2C_Slave.callback != NULL)
                    {
                        Soft_I2C_Slave.callback(Slave_Read_Request);
                    }
                }
                /* if slave is ready set, Byte_Count in callaback*/
                if (Soft_I2C_Slave.Byte_Count)
                {
                    Soft_I2C_Slave.state = Slave_Give_ACK;
                }
                else
                {
                    Soft_I2C_Slave.state = Slave_Detect_Stop;
                }
            }
        }
        else if (Soft_I2C_Slave.state == Slave_Give_ACK)
        {
            if (Soft_I2C_Slave.Received_Address == Soft_I2C_Slave.Own_Address)
            {
                /** write request */
                Soft_I2C_Slave.state = Slave_Data_In;
            }
            else
            {
        	/**(Own_Address + 1) read request */
                Soft_I2C_Slave.state = Slave_Data_Out;
            }
        }
        else if (Soft_I2C_Slave.state == Slave_Data_In)
        {
            if (Soft_I2C_Slave.Byte_Count)
            {
                Soft_I2C_Slave.Current_Byte <<= 1;

                if (Soft_I2C_Slave_SDA_Read())
                {
                    Soft_I2C_Slave.Current_Byte |= 0x01;
                }

                Soft_I2C_Slave.Bit_Count++;

                if (Soft_I2C_Slave.Bit_Count == 8)
                {
                    Soft_I2C_Slave.Bit_Count = 0;
                    Soft_I2C_Slave.Byte_Count--;
                    Soft_I2C_Slave.state = Slave_Give_ACK;
                    Soft_I2C_Slave.Buffer[Soft_I2C_Slave.Buffer_Index] = Soft_I2C_Slave.Current_Byte;
                    Soft_I2C_Slave.Buffer_Index++;
                    Soft_I2C_Slave.Current_Byte = 0;
                }
            }
            else
            {
                /* same as giving NACK*/
                Soft_I2C_Slave.state = Slave_Detect_Stop;
            }
        }
        else if (Soft_I2C_Slave.state == Slave_Data_Out)
        {
            if (Soft_I2C_Slave.Bit_Count == 8)
            {
                Soft_I2C_Slave.Bit_Count = 0;
                Soft_I2C_Slave.Current_Byte = 0;
                Soft_I2C_Slave.state = Slave_Read_ACK;
            }
        }
        else if (Soft_I2C_Slave.state == Slave_Read_ACK)
        {
            if (Soft_I2C_Slave_ACK_Read())
            {
                Soft_I2C_Slave.state = Slave_Data_Out;
                if (Soft_I2C_Slave.callback != NULL)
                {
                    Soft_I2C_Slave.callback(Slave_ACK_Received);
                }
                /* grab byte from buffer to send */
                Soft_I2C_Slave.Current_Byte = Soft_I2C_Slave.Buffer[Soft_I2C_Slave.Buffer_Index];
                Soft_I2C_Slave.Buffer_Index++;
                if (Soft_I2C_Slave.Buffer_Index == Soft_I2C_Slave.Byte_Count)
                {
                    /* roll over */
                    Soft_I2C_Slave.Buffer_Index = 0;
                }
            }
            else
            {
                if (Soft_I2C_Slave.callback != NULL)
                {
                    Soft_I2C_Slave.callback(Slave_NACK_Received);
                }
                Soft_I2C_Slave.state = Slave_Detect_Stop;
            }
        }
    }
    else
    {
        /*falling edge edge*/
        if (Soft_I2C_Slave.state == Slave_Detect_Start || Soft_I2C_Slave.state == Slave_Detect_Stop)
        {
            /** nothing to do here */
            (void)0;
        }
        else if (Soft_I2C_Slave.state == Slave_Address_In || Soft_I2C_Slave.state == Slave_Data_In || Soft_I2C_Slave.state == Slave_Read_ACK)
        {
            /* leave sda high */
            Soft_I2C_Slave_SDA_High();
        }
        else if (Soft_I2C_Slave.state == Slave_Give_ACK)
        {
            Soft_I2C_Slave_SDA_Low();
        }
        else if (Soft_I2C_Slave.state == Slave_Data_Out)
        {
            /*send Current_Byte*/
            if (Soft_I2C_Slave.Current_Byte & 0x80)
            {
                Soft_I2C_Slave_SDA_High();
            }
            else
            {
                Soft_I2C_Slave_SDA_Low();
            }

            Soft_I2C_Slave.Current_Byte <<= 1;
            Soft_I2C_Slave.Bit_Count++;
        }
    }
}

void EXTI9_5_IRQHandler(void)
{
    if (__HAL_GPIO_EXTI_GET_IT(SOFT_I2C_SLAVE_SDA_PIN) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(SOFT_I2C_SLAVE_SDA_PIN);
        Soft_I2C_Slave_State_Machine_SDA_ISR();
    }

    if (__HAL_GPIO_EXTI_GET_IT(SOFT_I2C_SLAVE_SCL_PIN) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(SOFT_I2C_SLAVE_SCL_PIN);
        Soft_I2C_Slave_State_Machine_SCL_ISR();
    }
}
