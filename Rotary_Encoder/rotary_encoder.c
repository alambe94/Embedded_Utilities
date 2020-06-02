#include "rotary_encoder.h"
// based of
// multi encoder code with speed up values adapted for STM32-arduino by Matthias Diro
// no caps for encoder Pins needed: gives back wrong acc values
// using gpio_read_bit instead of digitalRead gives about 4x more speed http://forums.leaflabs.com/topic.php?id=1107

#define MAX_ENCODERS 3

static Encoder_Struct_t *Encoder_List[MAX_ENCODERS];

static uint8_t Encoder_Count = 0;

extern uint32_t Encoder_Get_Tick();

uint8_t Encoder_Add(Encoder_Struct_t *handle)
{
    if (Encoder_Count < MAX_ENCODERS)
    {
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
    Encoder_Struct_t *handle = NULL;

    for (uint8_t Index = 0; Index < Encoder_Count; Index++)
    {
        handle = Encoder_List[Index];

        /*current state != previous*/
        if (handle->Encoder_Read_Pin_A() != handle->Encoder_Pin_A_State)
        {
            handle->Encoder_Pin_A_State = !handle->Encoder_Pin_A_State;

            if (handle->Encoder_Pin_A_State && !handle->Encoder_Pin_B_State)
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

        if (handle->Encoder_Read_Pin_B() != handle->Encoder_Pin_B_State)
        {
            handle->Encoder_Pin_B_State = !handle->Encoder_Pin_B_State;

            if (handle->Encoder_Pin_B_State && !handle->Encoder_Pin_A_State)
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

int16_t Encoder_Get_Count(Encoder_Struct_t *handle)
{
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
            /* copy count, encoder is still idle */
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
    if (handle != NULL)
    {
        handle->Encoder_Count = 0;
    }
}

void Encoder_Set_Count(Encoder_Struct_t *handle, int16_t count)
{
    if (handle != NULL)
    {
        handle->Encoder_Count = count;
    }
}
