#include "stdio.h"
#include "menu_core.h"

#define MAX_PAGES 15
#define REFRESH_CYCLE 20

#if (USE_MENU_ASSERT == 1)
#include "stdio.h"
#define MENU_ASSERT(expr, msg) ((expr) ? (void)0U : Menu_Assert(msg, "menu_core.c", __LINE__))
static void Menu_Assert(char *msg, char *file, uint32_t line)
{
    printf("%s, assertion failed, file=%s, line=%lu\n", msg, file, line);
}
#else
#define MENU_ASSERT(expr, msg) ((void)0U)
#endif

static uint8_t Refresh_Flag;

static Menu_Page_t *Menu_Page_List[MAX_PAGES];

static uint8_t Menu_Page_Count = 0;

Menu_Page_t *Current_Page;

extern uint32_t Menu_Get_Tick();

extern void Menu_Get_Event(Menu_Event_t *event);

uint8_t Menu_Add_Page(Menu_Page_t *page)
{
    MENU_ASSERT(page, "NULL Passed");
    MENU_ASSERT(Menu_Page_Count < MAX_PAGES, "MAX Page count reached");

    uint8_t xreturn = 1;

    if (Menu_Page_Count < MAX_PAGES && page != NULL)
    {
        MENU_ASSERT(page->Page_Screen_List, "Page_Screen_List not defined");

        if (page->Page_Screen_List != NULL)
        {
            for (uint8_t i = 0; i < page->Screens_In_Page; i++)
            {
                MENU_ASSERT(page->Page_Screen_List[i].Enter_Page_Screen, "Enter_Page_Screen not defined");
                MENU_ASSERT(page->Page_Screen_List[i].Show_Page_Screen, "Show_Page_Screen not defined");

                if (page->Page_Screen_List[i].Show_Page_Screen == NULL ||
                    page->Page_Screen_List[i].Show_Page_Screen == NULL)
                {
                    xreturn = 0;
                }
            }
        }
        else
        {
            xreturn = 0;
        }
    }
    else
    {
        xreturn = 0;
    }

    if (xreturn)
    {
        Menu_Page_List[Menu_Page_Count++] = page;
    }

    return xreturn;
}

void Menu_Change_Page(uint8_t page_no, uint8_t page_screen)
{
    MENU_ASSERT(page_no < MAX_PAGES, "Page out of index");

    if (page_no < MAX_PAGES)
    {
        Current_Page = Menu_Page_List[page_no];

        MENU_ASSERT(page_screen < Current_Page->Screens_In_Page, "Screen out of index");

        if (page_screen < Current_Page->Screens_In_Page)
        {
            Current_Page->Current_Screen = page_screen;
        }

        Refresh_Flag = 1;
    }
}

void Menu_Loop()
{
    static uint8_t in_page_loop = 1; // by default enter page0 screen0 (Home Screen).
    static uint32_t Scan_Time_Stamp = 0;
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
                menu_event.Encoder_Count = 0;
                menu_event.Enter_Button_Clicks = 0;
                in_page_loop = Current_Page->Page_Screen_List[Current_Page->Current_Screen].Enter_Page_Screen(&menu_event);
            }
            /* up is pressed or encoder incremented */
            else if (menu_event.Encoder_Count > 0)
            {
                Current_Page->Current_Screen += menu_event.Encoder_Count;
                if (Current_Page->Current_Screen >= Current_Page->Screens_In_Page)
                {
                    Current_Page->Current_Screen = Current_Page->Screens_In_Page - 1;
                }
                Refresh_Flag = 1;
            }
            /* down is pressed or encoder decremented */
            else if (menu_event.Encoder_Count < 0)
            {
                int16_t temp = Current_Page->Current_Screen + menu_event.Encoder_Count;

                if (temp < 0)
                {
                    Current_Page->Current_Screen = 0;
                }
                else
                {
                    Current_Page->Current_Screen = temp;
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
