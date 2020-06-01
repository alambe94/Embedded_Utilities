#include "pid.h"

#define MAX_PID_OUTPUT (1000)
#define MAX_INTEGRATION_ERROR (150)

static float PID_KP = 1000;
static float PID_KI = 10;
static float PID_KD = 500;
static float PID_Integration_Error = 0;
static float PID_Previous_Error = 0;

float PID_Calculate(float Error)
{
    float Output;

    Output = (float)(PID_KP * Error + PID_KI * PID_Integration_Error + PID_KD * (Error - PID_Previous_Error));

    if (PID_KI != 0 && Output < MAX_PID_OUTPUT)
    {
        PID_Integration_Error += Error;
        // Limit the maximum integration Error
        if (PID_Integration_Error > MAX_INTEGRATION_ERROR)
        {
            PID_Integration_Error = MAX_INTEGRATION_ERROR;
        }
        else if (PID_Integration_Error < -MAX_INTEGRATION_ERROR)
        {
            PID_Integration_Error = -MAX_INTEGRATION_ERROR;
        }
    }
    else
    {
        //PID_Integration_Error=0;
    }

    // Limit the maximum Output
    if (Output > MAX_PID_OUTPUT)
    {
        Output = MAX_PID_OUTPUT;
    }
    if (Output < 0)
    {
        Output = 0;
    }

    PID_Previous_Error = Error;

    return Output;
}

void PID_Reset_Integration_Error()
{
    PID_Integration_Error = 0;
}

void PID_Set_KP(float kp)
{
    PID_KP = kp;
}

void PID_Set_KI(float ki)
{
    PID_KI = ki;
}

void PID_Set_KD(float kd)
{
    PID_KD = kd;
}

float PID_Get_KP()
{
    return PID_KP;
}

float PID_Get_KI()
{
    return PID_KI;
}

float PID_Get_KD()
{
    return PID_KD;
}
