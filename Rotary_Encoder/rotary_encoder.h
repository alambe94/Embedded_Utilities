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

#ifndef ROTARY_ENCODER_H_
#define ROTARY_ENCODER_H_

/** std includes */
#include <stdint.h>

typedef struct Encoder_Struct_t
{
	uint8_t Encoder_Pin_A_State;
	uint8_t Encoder_Pin_B_State;
	int16_t Encoder_Count;
	uint32_t Encoder_Time_Stamp;

	/** enable clock, configure gpio as input */
	void (*Encoder_Init)(void);

	/** return pin A & pin B state */
	uint8_t (*Encoder_Read_Pin_A)(void);
	uint8_t (*Encoder_Read_Pin_B)(void);

} Encoder_Struct_t;

void Encoder_Loop(void);
void Encoder_Reset_Count(Encoder_Struct_t *handle);
void Encoder_Set_Count(Encoder_Struct_t *handle, int16_t count);
int16_t Encoder_Get_Count(Encoder_Struct_t *handle);
int8_t Encoder_Add(Encoder_Struct_t *Encoder_Struct_handle);

#endif /* ROTARY_ENCODER_H_ */
