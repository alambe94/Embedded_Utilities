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

#ifndef BUTTON_H_
#define BUTTON_H_

#include "stdint.h"

#define MAX_BUTTONS 5

/** enable disable assert */
#define BUTTON_USE_ASSERT 1

/** milliseconds in single tick */
#define BUTTON_MS_IN_TICK 1

/** Button_Loop() executes every BUTTON_SCAN_TICK */
#define BUTTON_SCAN_TICK (10 / BUTTON_MS_IN_TICK)

/** if released for BUTTON_DEBOUNCE_TICK, register click */
#define BUTTON_DEBOUNCE_TICK (50 / BUTTON_SCAN_TICK)

/** if released for BUTTON_CLICKED_TICK, capture count and call the callback if defned */
#define BUTTON_CLICKED_TICK (250 / BUTTON_SCAN_TICK)

/** if repressed within BUTTON_REPRESSED_TICK, increment count */
#define BUTTON_REPRESSED_TICK (150 / BUTTON_SCAN_TICK)

/** if pressed for BUTTON_LONG_PRESSED_TICK, set count to 255 to indicate long pressed */
#define BUTTON_LONG_PRESSED_TICK (1000 / BUTTON_SCAN_TICK)

typedef enum Button_Event_t
{
    Button_Idle = 0,

    /** still pressed */
    Button_Pressed,

    /** at least one clicked and still pressed */
    Button_Repressed,
    Button_Long_Pressed

} Button_Event_t;

typedef struct Button_Struct_t
{
    Button_Event_t Button_Event;
    uint32_t Button_Pressed_Ticks;
    uint32_t Button_Released_Ticks;

    /** used in Callback if defined */
    uint8_t Button_Clicked_Count;

    /** used for manual read in Button_Get_Clicked_Count */
    uint8_t Button_Count_Captured;

    /** enable clock, configure gpio pin as input */
    void (*Button_Init)(void);

    /** return if button is pressed or not */
    uint8_t (*Button_Read)(void);

    /** optional callback function, if defined will be called after button event */
    void (*Callback)(uint8_t Button_Clicked_Count);

} Button_Struct_t;

void Button_Loop(void);
void Button_Reset_Count(Button_Struct_t *handle);
uint8_t Button_Add(Button_Struct_t *handle);
uint8_t Button_Get_Clicked_Count(Button_Struct_t *handle);
Button_Event_t Button_Get_Status(Button_Struct_t *handle);

#endif /* BUTTON_H_ */
