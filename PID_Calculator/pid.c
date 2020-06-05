#include "pid.h"

float PID_Calculate(PID_t *handle, float Error)
{
    float Output;

    Output = (float)((handle->PID_KP * Error) +
                     (handle->PID_KI * handle->PID_Integration_Error) +
                     (handle->PID_KD * (Error - handle->PID_Previous_Error)));

    if (handle->PID_KI != 0 && Output < handle->PID_MAX_Output)
    {
        handle->PID_Integration_Error += Error;
        // Limit the maximum integration Error
        if (handle->PID_Integration_Error > handle->PID_MAX_Integration_Error)
        {
            handle->PID_Integration_Error = handle->PID_MAX_Integration_Error;
        }
        else if (handle->PID_Integration_Error < -handle->PID_MAX_Integration_Error)
        {
            handle->PID_Integration_Error = -handle->PID_MAX_Integration_Error;
        }
    }
    else
    {
        //handle->PID_Integration_Error=0;
    }

    // Limit the maximum Output
    if (Output > handle->PID_MAX_Output)
    {
        Output = handle->PID_MAX_Output;
    }
    if (Output < 0)
    {
        Output = 0;
    }

    handle->PID_Previous_Error = Error;

    return Output;
}

void PID_Reset_Integration_Error(PID_t *handle)
{
    handle->PID_Integration_Error = 0;
}

void PID_Set_KP(PID_t *handle, float kp)
{
    handle->PID_KP = kp;
}

void PID_Set_KI(PID_t *handle, float ki)
{
    handle->PID_KI = ki;
}

void PID_Set_KD(PID_t *handle, float kd)
{
    handle->PID_KD = kd;
}

float PID_Get_KP(PID_t *handle)
{
    return handle->PID_KP;
}

float PID_Get_KI(PID_t *handle)
{
    return handle->PID_KI;
}

float PID_Get_KD(PID_t *handle)
{
    return handle->PID_KD;
}
