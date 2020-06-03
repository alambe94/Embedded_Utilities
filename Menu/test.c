#include "stdio.h"
#include "sys/timeb.h"
#include "menu_core.h"

uint64_t Menu_Get_Tick()
{
#if defined(_WIN32)
    struct _timeb timebuffer;
    _ftime(&timebuffer);
#else
    struct timeb timebuffer;
    ftime(&timebuffer);
#endif

    return (uint64_t)(((timebuffer.time * 1000) + timebuffer.millitm));
}

void Menu_Get_Event(Menu_Event_t *event)
{
    event->Encoder_Count = 0;
    event->Enter_Button_Clicks = 0;
    event->Up_Button_Clicks = 0;
    event->Down_Button_Clicks = 0;
    event->Right_Button_Clicks = 0;
    event->Left_Button_Clicks = 0;
}

void main()
{
    Menu_Init();

    while (1)
    {
        Menu_Loop();
    }
}