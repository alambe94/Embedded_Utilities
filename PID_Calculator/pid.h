#ifndef PID_H_
#define PID_H_

#include "stdint.h"

typedef struct PID_t
{
    float PID_KP;
    float PID_KI;
    float PID_KD;
    float PID_Previous_Error;
    float PID_MAX_Output;
    float PID_Integration_Error;
    float PID_MAX_Integration_Error;

} PID_t;

float PID_Calculate(PID_t *handle, float Error);
void PID_Reset_Integration_Error(PID_t *handle);
void PID_Set_KP(PID_t *handle, float kp);
void PID_Set_KI(PID_t *handle, float ki);
void PID_Set_KD(PID_t *handle, float kd);
float PID_Get_KP(PID_t *handle);
float PID_Get_KI(PID_t *handle);
float PID_Get_KD(PID_t *handle);

#endif /* PID_H_ */
