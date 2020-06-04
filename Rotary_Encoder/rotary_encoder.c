#include "rotary_encoder.h"
#include "stddef.h"

// based of
// multi encoder code with speed up values adapted for STM32-arduino by Matthias Diro
// no caps for encoder Pins needed: gives back wrong acc values
// using gpio_read_bit instead of digitalRead gives about 4x more speed http://forums.leaflabs.com/topic.php?id=1107

#define MAX_ENCODERS 3

#define ENCODER_SCAN_TICK 1                           // Encoder_Loop() executes every ENCODER_SCAN_TICK
#define ENCODER_READ_TICK (250 / ENCODER_SCAN_TICK)   // only read after 100ms of inactivity
#define ENCODER_RESET_TICK (1000 / ENCODER_SCAN_TICK) // reset encoder count after 500ms of inactivity

static Encoder_Struct_t *Encoder_List[MAX_ENCODERS];

static uint8_t Encoder_Count = 0;

/* must be defined externally by user  */
extern uint32_t Encoder_Get_Tick();

uint8_t Encoder_Add(Encoder_Struct_t *handle)
{
    ENCODER_ASSERT(handle, "NULL Passed");

    if (Encoder_Count < MAX_ENCODERS && handle != NULL)
    {
        ENCODER_ASSERT(handle->Encoder_Read_Pin_A, "NULL Passed");
        ENCODER_ASSERT(handle->Encoder_Read_Pin_B, "NULL Passed");

        /* Init function can be NULL, if gpio is initialized  */
        if (handle->Encoder_Init)
        {
            handle->Encoder_Init();
        }

        handle->Encoder_Time_Stamp = 0;
        handle->Encoder_Count = 0;

        /* initial state of encoder pins */
        if (handle->Encoder_Read_Pin_A && handle->Encoder_Read_Pin_B)
        {
            handle->Encoder_Pin_A_State = handle->Encoder_Read_Pin_A();
            handle->Encoder_Pin_B_State = handle->Encoder_Read_Pin_B();
        }

        Encoder_List[Encoder_Count] = handle;
        Encoder_Count++;

        return (Encoder_Count - 1); //return encoder ID
    }
    else
    {
        //Error
        return (255); //return error
    }
}

/* call in millis callback or systick callback */
void Encoder_Loop()
{
    static uint32_t Encoder_Scan_Time_Stamp = 0;
    Encoder_Struct_t *handle = NULL;
    uint8_t pin_a_new_state = 0;
    uint8_t pin_b_new_state = 0;

    /* execute this loop on every ENCODER_SCAN_TICK */
    if (Encoder_Get_Tick() - Encoder_Scan_Time_Stamp > ENCODER_SCAN_TICK)
    {
        Encoder_Scan_Time_Stamp = Encoder_Get_Tick();

        for (uint8_t Index = 0; Index < Encoder_Count; Index++)
        {
            handle = Encoder_List[Index];

            ENCODER_ASSERT(handle, "NULL found in list");

            if (handle != NULL)
            {
                pin_a_new_state = handle->Encoder_Read_Pin_A();
                pin_b_new_state = handle->Encoder_Read_Pin_B();

                /*current state != previous*/
                if (pin_a_new_state != handle->Encoder_Pin_A_State)
                {
                    handle->Encoder_Pin_A_State = pin_a_new_state;

                    if (handle->Encoder_Pin_A_State != handle->Encoder_Pin_B_State)
                    {
                        if (Encoder_Get_Tick() - handle->Encoder_Time_Stamp > 10)
                        {
                            handle->Encoder_Count += 1;
                        }
                        else if (Encoder_Get_Tick() - handle->Encoder_Time_Stamp > 5)
                        {
                            handle->Encoder_Count += 10;
                        }
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
                        if (Encoder_Get_Tick() - handle->Encoder_Time_Stamp > 10)
                        {
                            handle->Encoder_Count -= 1;
                        }
                        else if (Encoder_Get_Tick() - handle->Encoder_Time_Stamp > 5)
                        {
                            handle->Encoder_Count -= 10;
                        }
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

int16_t Encoder_Get_Count(Encoder_Struct_t *handle)
{
    ENCODER_ASSERT(handle, "NULL Passed");

    int16_t count = 0;

    if (handle != NULL)
    {
        if (Encoder_Get_Tick() - handle->Encoder_Time_Stamp > 500)
        {
            /* reset encoder count after 500ms of inactivity*/
            handle->Encoder_Count = 0;
        }
        else if (Encoder_Get_Tick() - handle->Encoder_Time_Stamp > 100)
        {
            /* copy count, only after 100ms of inactivity*/
            count = handle->Encoder_Count;
            handle->Encoder_Count = 0;
        }
        else
        {
            /* do nothing, encoder is still active */
        }
    }

    return count;
}

/* to reset*/
void Encoder_Reset_Count(Encoder_Struct_t *handle)
{
    ENCODER_ASSERT(handle, "NULL Passed");

    if (handle != NULL)
    {
        handle->Encoder_Count = 0;
    }
}

void Encoder_Set_Count(Encoder_Struct_t *handle, int16_t count)
{
    ENCODER_ASSERT(handle, "NULL Passed");

    if (handle != NULL)
    {
        handle->Encoder_Count = count;
    }
}

#ifdef USE_ENCODER_ASSERT
#include "stdio.h"
void Encoder_Assert(char *msg, char *file, uint32_t line)
{
    printf("%s, assertion failed, file=%s, line=%lu\n", msg, file, line);
}
#endif
