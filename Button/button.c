#include "button.h"
#include "stddef.h"

/**
 * @brief assert implemenation, set USE_BUTTON_ASSERT to 1 to enable assert
 **/
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

/**
 * @brief list of all registered buttons
 **/
static Button_Struct_t *Button_List[MAX_BUTTONS];

/**
 * @brief count of registred buttons
 **/
static uint8_t Button_Count = 0;

/**
 * @brief need to define by user externally. used for timing purpose. simply return millis or tick elapsed
 **/
extern uint32_t Button_Get_Tick();

/**
 * @brief add given button to list of registered buttons
 * @param handle handle of button to be registered
 * @retval return button ID (index+1  of button in registred list)
 * @note adjust MAX_BUTTONS accordingly
 **/
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
        handle->Button_Event = Button_Idle;
        handle->Button_Clicked_Count = 0;
        handle->Button_Pressed_Ticks = 0;
        handle->Button_Released_Ticks = 0;

        Button_List[Button_Count] = handle;

        Button_Count++;

        /** return button ID */
        return Button_Count;
    }

    return 0;
}

/**
 * @brief frequently called in main or timer ISR. should be called at least every 20ms?
 * @param none
 * @retval none
 **/
void Button_Loop()
{
    static uint32_t Button_Scan_Time_Stamp = 0;

    /** execute this loop on every BUTTON_SCAN_TICK */
    if (Button_Get_Tick() - Button_Scan_Time_Stamp > BUTTON_SCAN_TICK)
    {
        Button_Struct_t *handle = NULL;
        Button_Scan_Time_Stamp = Button_Get_Tick();

        for (uint8_t Index = 0; Index < Button_Count; Index++)
        {
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

                    /** button was pressed for BUTTON_DEBOUNCE_TICK */
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

                    if (handle->Button_Released_Ticks > BUTTON_CLICKED_TICK)
                    {
                        if (handle->Button_Event != Button_Idle)
                        {
                            handle->Button_Event = Button_Idle;
                            handle->Button_Count_Captured = handle->Button_Clicked_Count;
                            /** if button callback is defined, call it*/
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

/**
 * @brief return the current state of button
 * @param handle handle of button
 * @retval return Button_Event
 * @note if handle is NULL, returns 0, which same as Button_Idle
 **/
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
 * @note if handle is NULL, returns 0
 **/
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
 **/
void Button_Reset_Count(Button_Struct_t *handle)
{
    BUTTON_ASSERT(handle, "NULL Passed");

    if (handle != NULL)
    {
        handle->Button_Clicked_Count = 0;
        handle->Button_Count_Captured = 0;
    }
}
