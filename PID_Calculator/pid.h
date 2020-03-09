#ifndef PID_H_
#define PID_H_

float PID_Calculate(float Error);
void PID_Reset_Integration_Error();
void PID_Set_KP(float kp);
void PID_Set_KI(float ki);
void PID_Set_KD(float kd);
float PID_Get_KP();
float PID_Get_KI();
float PID_Get_KD();

#endif /* PID_H_ */
