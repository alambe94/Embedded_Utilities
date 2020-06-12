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

#ifndef PID_H_
#define PID_H_

/** std includes */
#include <stdint.h>

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
