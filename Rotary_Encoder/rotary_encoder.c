/**
 *  multi encoder code with speed up values adapted for STM32-arduino by Matthias Diro
 *  http://forums.leaflabs.com/topic.php?id=1107
 */
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

/** std includes */
#include <stddef.h>

/** std includes */
#include "rotary_encoder.h"

#define MAX_ENCODERS 3

/** enable disable assert */
#define ENCODER_USE_ASSERT 1

/** milliseconds in single tick */
#define ENCODER_MS_IN_TICK 1

/** Encoder_Loop() executes every ENCODER_SCAN_TICK */
#define ENCODER_SCAN_TICK (1 / ENCODER_MS_IN_TICK)

/** only read after ENCODER_READ_TICK of inactivity */
#define ENCODER_READ_TICK (250 / ENCODER_SCAN_TICK)

/** reset encoder count after ENCODER_RESET_TICK of inactivity */
#define ENCODER_RESET_TICK (1000 / ENCODER_SCAN_TICK)

/**
 * @brief assert implemenation, set ENCODER_USE_ASSERT to 1 to enable assert
 */
#if (ENCODER_USE_ASSERT == 1)
#include "stdio.h"
#define ENCODER_ASSERT(expr, msg) ((expr) ? (void)0U : Encoder_Assert(msg, "rotary_encoder.c", __LINE__))
static void Encoder_Assert(char *msg, char *file, uint32_t line)
{
    printf("%s, assertion failed, file=%s, line=%lu\n", msg, file, line);
}
#else
#define ENCODER_ASSERT(expr, msg) ((void)0U)
#endif

/**
 * @brief list of all registered encoders
 */
static Encoder_Struct_t *Encoder_List[MAX_ENCODERS];

/**
 * @brief count of registred encoders
 */
static uint8_t Encoder_Count = 0;

/**
 * @brief need to define by user externally. used for timing purpose. simply return millis or tick elapsed
 */
extern uint32_t Encoder_Get_Tick();

/**
 * @brief add given encoder to list of registered encoders
 * @param handle handle of encoder to be registered
 * @retval return encoder ID (index of encoder in registred list)
 * @note adjust MAX_ENCODERS accordingly
 */
int32_t Encoder_Add(Encoder_Struct_t *handle)
{
    ENCODER_ASSERT(handle, "handle is NULL");
    ENCODER_ASSERT(Encoder_Count < MAX_ENCODERS, "MAX Encoder count reached");

    if (Encoder_Count < MAX_ENCODERS && handle != NULL)
    {
        ENCODER_ASSERT(handle->Encoder_Read_Pin_A, "Encoder_Read_Pin_A not defined");
        ENCODER_ASSERT(handle->Encoder_Read_Pin_B, "Encoder_Read_Pin_B not defined");

        /** call init function if defined */
        if (handle->Encoder_Init != NULL)
        {
            handle->Encoder_Init();
        }

        handle->Encoder_Time_Stamp = 0;
        handle->Encoder_Count = 0;

        /** read initial state of encoder pins */
        if (handle->Encoder_Read_Pin_A != NULL && handle->Encoder_Read_Pin_B != NULL)
        {
            handle->Encoder_Pin_A_State = handle->Encoder_Read_Pin_A();
            handle->Encoder_Pin_B_State = handle->Encoder_Read_Pin_B();
        }

        Encoder_List[Encoder_Count] = handle;
        Encoder_Count++;

        /** return encoder ID. index of encoder in list */
        return Encoder_Count;
    }
    /** return error */
    return -1;
}

/**
 * @brief frequently called in main or timer ISR. should be called at least every 5ms?
 * @param none
 * @retval none
 */
void Encoder_Loop(void)
{
    static uint32_t Encoder_Scan_Time_Stamp = 0;
    Encoder_Struct_t *handle = NULL;

    /** execute this loop on every ENCODER_SCAN_TICK */
    if (Encoder_Get_Tick() - Encoder_Scan_Time_Stamp > ENCODER_SCAN_TICK)
    {
        Encoder_Scan_Time_Stamp = Encoder_Get_Tick();

        for (uint8_t Index = 0; Index < Encoder_Count; Index++)
        {
            handle = Encoder_List[Index];

            ENCODER_ASSERT(handle, "NULL found in list");

            if (handle != NULL)
            {
                uint8_t pin_a_new_state = handle->Encoder_Read_Pin_A();
                uint8_t pin_b_new_state = handle->Encoder_Read_Pin_B();

                /** current state != previous */
                if (pin_a_new_state != handle->Encoder_Pin_A_State)
                {
                    handle->Encoder_Pin_A_State = pin_a_new_state;

                    if (handle->Encoder_Pin_A_State != handle->Encoder_Pin_B_State)
                    {
                        /** encoder velocity normal, last transition > 10ms */
                        if (Encoder_Get_Tick() - handle->Encoder_Time_Stamp > 10)
                        {
                            handle->Encoder_Count += 1;
                        }
                        /** encoder velocity fast, last transition 5ms < transition < 10ms */
                        else if (Encoder_Get_Tick() - handle->Encoder_Time_Stamp > 5)
                        {
                            handle->Encoder_Count += 10;
                        }
                        /** encoder velocity faster, last transition within 5ms */
                        else
                        {
                            handle->Encoder_Count += 50;
                        }

                        handle->Encoder_Time_Stamp = Encoder_Get_Tick();
                    }
                }

                if (pin_b_new_state != handle->Encoder_Pin_B_State)
                {
                    handle->Encoder_Pin_B_State = pin_b_new_state;

                    if (handle->Encoder_Pin_B_State != handle->Encoder_Pin_A_State)
                    {
                        /** encoder velocity normal, last transition > 10ms */
                        if (Encoder_Get_Tick() - handle->Encoder_Time_Stamp > 10)
                        {
                            handle->Encoder_Count -= 1;
                        }
                        /** encoder velocity fast, last transition 5ms < transition < 10ms */
                        else if (Encoder_Get_Tick() - handle->Encoder_Time_Stamp > 5)
                        {
                            handle->Encoder_Count -= 10;
                        }
                        /** encoder velocity faster, last transition within 5ms */
                        else
                        {
                            handle->Encoder_Count -= 50;
                        }

                        handle->Encoder_Time_Stamp = Encoder_Get_Tick();
                    }
                }
            }
        }
    }
}

/**
 * @brief return the count of encoder
 * @param handle handle of encoder
 * @retval return encoder count
 * @note if handle is NULL, returns 0
 */
int32_t Encoder_Get_Count(Encoder_Struct_t *handle)
{
    ENCODER_ASSERT(handle, "handle is NULL");

    int32_t count = 0;

    if (handle != NULL)
    {
        if (Encoder_Get_Tick() - handle->Encoder_Time_Stamp > 500)
        {
            /** reset encoder count after 500ms of inactivity */
            handle->Encoder_Count = 0;
        }
        else if (Encoder_Get_Tick() - handle->Encoder_Time_Stamp > 100)
        {
            /** copy count, only after 100ms of inactivity */
            count = handle->Encoder_Count;
            handle->Encoder_Count = 0;
        }
        else
        {
            /** do nothing, encoder is still active (in transition) */
        }
    }

    return count;
}

/**
 * @brief reset the count of encoder
 * @param handle handle of encoder
 */
void Encoder_Reset_Count(Encoder_Struct_t *handle)
{
    ENCODER_ASSERT(handle, "handle is NULL");

    if (handle != NULL)
    {
        handle->Encoder_Count = 0;
    }
}

/**
 * @brief set the count of encoder
 * @param handle handle of encoder
 */
void Encoder_Set_Count(Encoder_Struct_t *handle, int16_t count)
{
    ENCODER_ASSERT(handle, "handle is NULL");

    if (handle != NULL)
    {
        handle->Encoder_Count = count;
    }
}
