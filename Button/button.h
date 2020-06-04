#ifndef BUTTON_H_
#define BUTTON_H_

#include "stdint.h"

#define USE_BUTTON_ASSERT

typedef enum Button_Event_t
{
    Button_Idle = 0,
    Button_Pressed,   // still pressed
    Button_Repressed, // at least one clicked and still pressed
    Button_Long_Pressed

} Button_Event_t;

typedef struct Button_Struct_t
{
    Button_Event_t Button_Event;
    uint32_t Button_Pressed_Ticks;
    uint32_t Button_Released_Ticks;
    uint8_t Button_Clicked_Count;                   // used in Callback
    uint8_t Button_Count_Captured;                  // used for manual read in Button_Get_Clicked_Count
    void (*Button_Init)(void);                      // enable clock, configure as input
    uint8_t (*Button_Read)(void);                   // return button state
    void (*Callback)(uint8_t Button_Clicked_Count); // callback function ptr

} Button_Struct_t;

void Button_Loop();
void Button_Reset_Count(Button_Struct_t *handle);
uint8_t Button_Add(Button_Struct_t *handle);
uint8_t Button_Get_Clicked_Count(Button_Struct_t *handle);
Button_Event_t Button_Get_Status(Button_Struct_t *handle);

#ifdef USE_BUTTON_ASSERT
void Button_Assert(char *msg, char *file, uint32_t line);
#define BUTTON_ASSERT(expr, msg) ((expr) ? (void)0U : Button_Assert(msg, "button.c", __LINE__))
#else
#define BUTTON_ASSERT(expr) ((void)0U)
#endif

#endif /* BUTTON_H_ */
