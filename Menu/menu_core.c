#include "stdio.h"
#include "menu_core.h"
#include "menu_page0.h"
#include "menu_page1.h"

#define MAX_PAGES 10
#define REFRESH_CYCLE 20

static uint8_t Refresh_Flag;

static Menu_Page_t *Menu_Page_List[MAX_PAGES];

Menu_Page_t *Current_Page;

extern uint32_t Menu_Get_Tick();

extern void Menu_Get_Event(Menu_Event_t *event);

void Menu_Add_Page(Menu_Page_t *page)
{
    static uint8_t Menu_Page_Count;

    if (Menu_Page_Count < MAX_PAGES)
    {
        Menu_Page_List[Menu_Page_Count++] = page;
    }
}

void Menu_Change_Page(uint8_t page_no, uint8_t page_screen)
{
    if (page_no < MAX_PAGES)
    {
        Current_Page = Menu_Page_List[page_no];

        if (page_screen < Current_Page->Screens_In_Page)
        {
            Current_Page->Current_Screen = page_screen;
        }

        Refresh_Flag = 1;
    }
}

void Menu_Init()
{
    Menu_Page0_Init();
    Menu_Page1_Init();
    Menu_Change_Page(0, 0); // by default show page0 screen0.
}

void Menu_Loop()
{
    static uint8_t in_page_loop = 1; // by default enter page0 screen0 (Home Screen).
    static uint64_t Scan_Time_Stamp = 0;
    Menu_Event_t menu_event;

    if (Menu_Get_Tick() - Scan_Time_Stamp > (REFRESH_CYCLE - 1))
    {
        Scan_Time_Stamp = Menu_Get_Tick();

        Menu_Get_Event(&menu_event);

        if (in_page_loop)
        {
            in_page_loop = Current_Page->Page_Screen_List[Current_Page->Current_Screen].Enter_Page_Screen(&menu_event);

            if (!in_page_loop)
            {
                Refresh_Flag = 1;
            }
        }
        else
        {
            /* enter button or select button is pressed */
            if (menu_event.Enter_Button_Clicks == 1)
            {
                /* click and count do not belong to page loop so reset them */
                menu_event.Enter_Button_Clicks = 0;
                menu_event.Encoder_Count = 0;
                in_page_loop = Current_Page->Page_Screen_List[Current_Page->Current_Screen].Enter_Page_Screen(&menu_event);
            }
            /* down is pressed or encoder decremented */
            else if (menu_event.Encoder_Count < 0)
            {
                Current_Page->Current_Screen++;
                if (Current_Page->Current_Screen >= Current_Page->Screens_In_Page)
                {
                    Current_Page->Current_Screen = Current_Page->Screens_In_Page - 1;
                }
                Refresh_Flag = 1;
            }
            /* up is pressed or encoder incremented */
            else if (menu_event.Encoder_Count > 0)
            {
                if (Current_Page->Current_Screen)
                {
                    Current_Page->Current_Screen--;
                }
                Refresh_Flag = 1;
            }

            if (Refresh_Flag == 1)
            {
                Refresh_Flag = 0;
                Current_Page->Page_Screen_List[Current_Page->Current_Screen].Show_Page_Screen();
            }
        }
    }
}
