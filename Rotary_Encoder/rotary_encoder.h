#ifndef ROTARY_ENCODER_H_
#define ROTARY_ENCODER_H_

#include "stdint.h"

#define HIGH 1
#define LOW 0

typedef enum Encoder_State_t
{
	Encoder_State_00 = 0,
	Encoder_State_01,
	Encoder_State_10,
	Encoder_State_11

} Encoder_State_t;

typedef struct Encoder_Struct_t
{
	uint8_t Encoder_Pin_A_State;
	uint8_t Encoder_Pin_B_State;
	uint32_t Encoder_Time_Stamp;
	int16_t Encoder_Count;
	void (*Encoder_Init)(void);			 // enable clock, configure as gpio as input
	uint8_t (*Encoder_Read_Pin_A)(void); // return pin A state
	uint8_t (*Encoder_Read_Pin_B)(void); // return pin A state

} Encoder_Struct_t;

void Encoder_Loop();
int16_t Encoder_Get_Count(Encoder_Struct_t *handle);
void Encoder_Reset_Count(Encoder_Struct_t *handle);
void Encoder_Set_Count(Encoder_Struct_t *handle, int16_t count);

uint8_t Encoder_Add(Encoder_Struct_t *Encoder_Struct_handle);

#endif /* ROTARY_ENCODER_H_ */
