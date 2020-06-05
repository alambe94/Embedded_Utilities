#include "button.h"
#include "stddef.h"

#define MAX_BUTTONS 5

#define BUTTON_SCAN_TICK 10 // Button_Loop() executes every BUTTON_SCAN_TICK

#define BUTTON_DEBOUNCE_DELAY (50 / BUTTON_SCAN_TICK)       // if ressed for BUTTON_DEBOUNCE_DELAY register click
#define BUTTON_CLICKED_DELAY (250 / BUTTON_SCAN_TICK)       // if released for BUTTON_CLICKED_DELAY, capture count and call the callback if defned
#define BUTTON_REPRESSED_DELAY (150 / BUTTON_SCAN_TICK)     // if repressed within BUTTON_REPRESSED_DELAY increment count
#define BUTTON_LONG_PRESSED_DELAY (1000 / BUTTON_SCAN_TICK) // if pressed for BUTTON_LONG_PRESSED_DELAY set count to 255 to indicate long pressed

#if (USE_BUTTON_ASSERT == 1)
#include "stdio.h"
#define BUTTON_ASSERT(expr, msg) ((expr) ? (void)0U : Button_Assert(msg, "button.c", __LINE__))
void Button_Assert(char *msg, char *file, uint32_t line)
{
    printf("%s, assertion failed, file=%s, line=%lu\n", msg, file, line);
}
#else
#define BUTTON_ASSERT(expr, msg) ((void)0U)
#endif

static Button_Struct_t *Button_List[MAX_BUTTONS];

static uint8_t Button_Count = 0;

extern uint32_t Button_Get_Tick();

uint8_t Button_Add(Button_Struct_t *handle)
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
        handle->Button_Clicked_Count = 0;
        handle->Button_Event = Button_Idle;
        handle->Button_Pressed_Ticks = 0;
        handle->Button_Released_Ticks = 0;

        Button_List[Button_Count] = handle;

        Button_Count++;

        return (Button_Count - 1); //return button ID
    }
    else
    {
        //return error
        return (255);
    }
}

/**
 * called every tick (BUTTON_SCAN_TICK ?)
 * should be called at least every 20ms?
 **/
void Button_Loop()
{
    static uint32_t Button_Scan_Time_Stamp = 0;
    Button_Struct_t *handle = NULL;

    /* execute this loop on every BUTTON_SCAN_TICK */
    if (Button_Get_Tick() - Button_Scan_Time_Stamp > (BUTTON_SCAN_TICK - 1))
    {
        Button_Scan_Time_Stamp = Button_Get_Tick();

        for (uint8_t Index = 0; Index < Button_Count; Index++)
        {
            handle = Button_List[Index];

            BUTTON_ASSERT(handle, "NULL found in list");

            if (handle != NULL)
            {
                if (handle->Button_Read != NULL && handle->Button_Read()) //pressed detected
                {
                    handle->Button_Pressed_Ticks++; //

                    if (handle->Button_Pressed_Ticks < BUTTON_LONG_PRESSED_DELAY)
                    {
                        //repressed detected
                        if (handle->Button_Released_Ticks < BUTTON_REPRESSED_DELAY)
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
                        //long pressed detected
                        handle->Button_Event = Button_Long_Pressed;
                        handle->Button_Clicked_Count = 0xFF; //0xFF for long press
                        handle->Button_Count_Captured = 0xFF;
                        if (handle->Callback != NULL)
                        {
                            handle->Callback(handle->Button_Clicked_Count);
                            handle->Button_Clicked_Count = 0;
                        }
                    }
                }
                else //released detected
                {
                    handle->Button_Released_Ticks++;

                    if (handle->Button_Pressed_Ticks > BUTTON_DEBOUNCE_DELAY) //if button was pressed for BUTTON_DEBOUNCE_DELAY
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

                    if (handle->Button_Released_Ticks > BUTTON_CLICKED_DELAY)
                    {
                        if (handle->Button_Event != Button_Idle)
                        {
                            handle->Button_Event = Button_Idle;
                            handle->Button_Count_Captured = handle->Button_Clicked_Count;
                            /* if button callback is defined. call*/
                            if (handle->Callback != NULL)
                            {
                                handle->Callback(handle->Button_Clicked_Count);
                            }
                            handle->Button_Clicked_Count = 0;
                        }
                    }
                }
            }
        }
    }
}

Button_Event_t Button_Get_Status(Button_Struct_t *handle)
{
    BUTTON_ASSERT(handle, "NULL Passed");

    if (handle != NULL)
    {
        return handle->Button_Event;
    }
    return 255; // Error
}

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

void Button_Reset_Count(Button_Struct_t *handle)
{
    BUTTON_ASSERT(handle, "NULL Passed");

    if (handle != NULL)
    {
        handle->Button_Clicked_Count = 0;
    }
}
