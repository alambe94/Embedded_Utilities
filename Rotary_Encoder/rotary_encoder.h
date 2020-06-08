#ifndef ROTARY_ENCODER_H_
#define ROTARY_ENCODER_H_

#include "stdint.h"

#define MAX_ENCODERS 3

/** enable disable assert */
#define USE_ENCODER_ASSERT 1

/** milliseconds in single tick */
#define ENCODER_MS_IN_TICK 1

/** Encoder_Loop() executes every ENCODER_SCAN_TICK */
#define ENCODER_SCAN_TICK (1 / ENCODER_MS_IN_TICK)

/** only read after ENCODER_READ_TICK of inactivity */
#define ENCODER_READ_TICK (250 / ENCODER_SCAN_TICK)

/** reset encoder count after ENCODER_RESET_TICK of inactivity */
#define ENCODER_RESET_TICK (1000 / ENCODER_SCAN_TICK)

typedef struct Encoder_Struct_t
{
	uint8_t Encoder_Pin_A_State;
	uint8_t Encoder_Pin_B_State;
	uint32_t Encoder_Time_Stamp;
	int16_t Encoder_Count;

	/** enable clock, configure as gpio as input */
	void (*Encoder_Init)(void);

	/** return pin A & pin Bstate */
	uint8_t (*Encoder_Read_Pin_A)(void);
	uint8_t (*Encoder_Read_Pin_B)(void);

} Encoder_Struct_t;

void Encoder_Loop();
void Encoder_Reset_Count(Encoder_Struct_t *handle);
void Encoder_Set_Count(Encoder_Struct_t *handle, int16_t count);
int16_t Encoder_Get_Count(Encoder_Struct_t *handle);
uint8_t Encoder_Add(Encoder_Struct_t *Encoder_Struct_handle);

#endif /* ROTARY_ENCODER_H_ */
