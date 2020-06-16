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

/** std includes */
#include <stddef.h>

/** button includes */
#include "button.h"

#define MAX_BUTTONS 5

/** enable/disable assert */
#define BUTTON_USE_ASSERT 1

/** milliseconds in single tick */
#define BUTTON_MS_IN_TICK 1

/** Button_Loop() executes every BUTTON_SCAN_TICK */
#define BUTTON_SCAN_TICK (10 / BUTTON_MS_IN_TICK)

/** if released for BUTTON_DEBOUNCE_TICK, register click */
#define BUTTON_DEBOUNCE_TICK (50 / BUTTON_SCAN_TICK)

/** if released for BUTTON_CLICKED_TICK, capture click count and call the callback if defned */
#define BUTTON_CLICKED_TICK (250 / BUTTON_SCAN_TICK)

/** if repressed within BUTTON_REPRESSED_TICK, increment count */
#define BUTTON_REPRESSED_TICK (150 / BUTTON_SCAN_TICK)

/** if pressed for BUTTON_LONG_PRESSED_TICK, set count to 255 to indicate long pressed */
#define BUTTON_LONG_PRESSED_TICK (1000 / BUTTON_SCAN_TICK)

/**
 * @brief assert implemenation, set BUTTON_USE_ASSERT to 1 to enable assert
 */
#if (BUTTON_USE_ASSERT == 1)
#include <stdio.h>
#define BUTTON_ASSERT(expr, msg) ((expr) ? (void)0U : Button_Assert(msg, "button.c", __LINE__))
void Button_Assert(char *msg, char *file, uint32_t line)
{
    printf("%s, assertion failed, file=%s, line=%lu\n", msg, file, line);
}
#else
#define BUTTON_ASSERT(expr, msg) ((void)0U)
#endif

/**
 * @brief list of all registered buttons
 */
static Button_Struct_t *Button_List[MAX_BUTTONS];

/**
 * @brief count of registred buttons
 */
static uint8_t Button_Count = 0;

/**
 * @brief need to define by user externally. used for timing purpose. simply return millis or tick elapsed
 */
extern uint32_t Button_Get_Tick();

/**
 * @brief add given button to list of registered buttons
 * @param handle handle of button to be registered
 * @retval return button ID (index of button in registred list), return -1 on failure
 * @note adjust MAX_BUTTONS accordingly
 */
int32_t Button_Add(Button_Struct_t *handle)
{
    BUTTON_ASSERT(handle, "NULL Passed");
    BUTTON_ASSERT(Button_Count < MAX_BUTTONS, "MAX Button count reached");

    if (Button_Count < MAX_BUTTONS && handle != NULL)
    {
        BUTTON_ASSERT(handle->Button_Read, "Button_Read not defined");

        /* call init function if defined */
        if (handle->Button_Init != NULL)
        {
            handle->Button_Init();
        }
        handle->Button_Event = Button_Idle;
        handle->Button_Clicked_Count = 0;
        handle->Button_Pressed_Ticks = 0;
        handle->Button_Released_Ticks = 0;

        Button_List[Button_Count] = handle;

        Button_Count++;

        /** return button ID (index of button in registred list) */
        return Button_Count - 1;
    }
    /** return error */
    return -1;
}

/**
 * @brief frequently called in main loop or timer ISR. should be called at least every 20ms?
 * @param none
 * @retval none
 */
void Button_Loop(void)
{
    static uint32_t Button_Scan_Time_Stamp = 0;

    /** execute this loop on every BUTTON_SCAN_TICK */
    if (Button_Get_Tick() - Button_Scan_Time_Stamp > BUTTON_SCAN_TICK)
    {
        Button_Struct_t *handle = NULL;
        Button_Scan_Time_Stamp = Button_Get_Tick();

        for (uint8_t Index = 0; Index < Button_Count; Index++)
        {
            /** grab button handle from list */
            handle = Button_List[Index];

            BUTTON_ASSERT(handle, "NULL found in list");

            if (handle != NULL)
            {
                /** button pressed detected */
                if (handle->Button_Read != NULL && handle->Button_Read())
                {
                    handle->Button_Pressed_Ticks++;

                    if (handle->Button_Pressed_Ticks < BUTTON_LONG_PRESSED_TICK)
                    {
                        /** button repressed detected */
                        if (handle->Button_Released_Ticks < BUTTON_REPRESSED_TICK)
                        {
                            handle->Button_Event = Button_Repressed;
                        }
                        else
                        {
                            handle->Button_Event = Button_Pressed;
                        }
                        handle->Button_Released_Ticks = 0;
                    }
                    else if (handle->Button_Event != Button_Long_Pressed)
                    {
                        /** button long pressed detected */
                        handle->Button_Event = Button_Long_Pressed;
                        /** 0xFF for long press */
                        handle->Button_Clicked_Count = 0xFF;
                        handle->Button_Count_Captured = 0xFF;
                        /** if button callback is defined, call it*/
                        if (handle->Callback != NULL)
                        {
                            handle->Callback(handle->Button_Clicked_Count);
                        }
                        handle->Button_Clicked_Count = 0;
                    }
                }
                else
                {
                    /** button released detected */
                    handle->Button_Released_Ticks++;

                    /** if button was pressed for BUTTON_DEBOUNCE_TICK */
                    if (handle->Button_Pressed_Ticks > BUTTON_DEBOUNCE_TICK)
                    {
                        handle->Button_Pressed_Ticks = 0;

                        if (handle->Button_Event == Button_Repressed)
                        {
                            handle->Button_Clicked_Count++;
                        }
                        else if (handle->Button_Event == Button_Pressed)
                        {
                            handle->Button_Clicked_Count = 1;
                        }
                        else if (handle->Button_Event == Button_Long_Pressed)
                        {
                            handle->Button_Event = Button_Idle;
                        }
                    }

                    /** if button released for BUTTON_CLICKED_TICK */
                    if (handle->Button_Released_Ticks > BUTTON_CLICKED_TICK)
                    {
                        if (handle->Button_Event != Button_Idle)
                        {
                            handle->Button_Event = Button_Idle;
                            /** capture the button click so we can read it manually if we want */
                            handle->Button_Count_Captured = handle->Button_Clicked_Count;
                            /** if button callback is defined, call it */
                            if (handle->Callback != NULL)
                            {
                                handle->Callback(handle->Button_Clicked_Count);
                            }
                            /** reest click count after callback and start the scan again */
                            handle->Button_Clicked_Count = 0;
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief return the current state of button
 * @param handle handle of button
 * @retval return Button_Event
 */
Button_Event_t Button_Get_Status(Button_Struct_t *handle)
{
    BUTTON_ASSERT(handle, "NULL Passed");

    if (handle != NULL)
    {
        return handle->Button_Event;
    }
    /** error*/
    return 0;
}

/**
 * @brief return the clicked count of button
 * @param handle handle of button
 * @retval return clicked count
 */
uint8_t Button_Get_Clicked_Count(Button_Struct_t *handle)
{
    BUTTON_ASSERT(handle, "NULL Passed");

    uint8_t count = 0;

    if (handle != NULL)
    {
        if (handle->Button_Event == Button_Idle || handle->Button_Event == Button_Long_Pressed)
        {
            count = handle->Button_Count_Captured;
            handle->Button_Count_Captured = 0;
        }
    }

    return count;
}

/**
 * @brief reset the clicked count of button
 * @param handle handle of button
 */
void Button_Reset_Count(Button_Struct_t *handle)
{
    BUTTON_ASSERT(handle, "NULL Passed");

    if (handle != NULL)
    {
        handle->Button_Clicked_Count = 0;
        handle->Button_Count_Captured = 0;
    }
}
