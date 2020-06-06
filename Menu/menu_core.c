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
        MENU_ASSERT(page->Page_Item_List, "Page_Item_List not defined");

        if (page->Page_Item_List != NULL)
        {
            for (uint8_t i = 0; i < page->Items_In_Page; i++)
            {
                MENU_ASSERT(page->Page_Item_List[i].Page_Item_Callback, "Page_Item_Callback not defined");
                MENU_ASSERT(page->Page_Item_List[i].Show_Page_Item, "Show_Page_Item not defined");

                if (page->Page_Item_List[i].Show_Page_Item == NULL ||
                    page->Page_Item_List[i].Page_Item_Callback == NULL)
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

void Menu_Change_Page(uint8_t page_no, uint8_t page_Item)
{
    MENU_ASSERT(page_no < MAX_PAGES, "Page out of index");

    if (page_no < MAX_PAGES)
    {
        Current_Page = Menu_Page_List[page_no];

        MENU_ASSERT(page_Item < Current_Page->Items_In_Page, "Item out of index");

        if (page_Item < Current_Page->Items_In_Page)
        {
            Current_Page->Current_Item = page_Item;
        }
    }
}

void Menu_Loop()
{
    static uint8_t call_page_callback = 1; // by default enter page0 Item0 (Home Item).
    static uint32_t Scan_Time_Stamp = 0;
    uint8_t refresh_flag = 0;
    Menu_Event_t menu_event;

    if (Menu_Get_Tick() - Scan_Time_Stamp > (REFRESH_CYCLE - 1))
    {
        Scan_Time_Stamp = Menu_Get_Tick();
        Menu_Get_Event(&menu_event);

        if (call_page_callback)
        {
            call_page_callback = Current_Page->Page_Item_List[Current_Page->Current_Item].Page_Item_Callback(&menu_event);
            if (!call_page_callback)
            {
                refresh_flag = 1;
            }
        }
        else
        {
            /* enter button or select button is pressed */
            if (menu_event.Enter_Button_Clicks == 1)
            {
                /* this click and count do not belong to page callback so reset them */
                menu_event.Encoder_Count = 0;
                menu_event.Enter_Button_Clicks = 0;
                call_page_callback = Current_Page->Page_Item_List[Current_Page->Current_Item].Page_Item_Callback(&menu_event);
                if (!call_page_callback)
                {
                    refresh_flag = 1;
                }
            }
            /* up is pressed or encoder incremented */
            else if (menu_event.Encoder_Count > 0)
            {
                Current_Page->Current_Item += menu_event.Encoder_Count;
                if (Current_Page->Current_Item >= Current_Page->Items_In_Page)
                {
                    Current_Page->Current_Item = Current_Page->Items_In_Page - 1;
                }
                refresh_flag = 1;
            }
            /* down is pressed or encoder decremented */
            else if (menu_event.Encoder_Count < 0)
            {
                int16_t temp = Current_Page->Current_Item + menu_event.Encoder_Count;
                if (temp < 0)
                {
                    Current_Page->Current_Item = 0;
                }
                else
                {
                    Current_Page->Current_Item = temp;
                }
                refresh_flag = 1;
            }
        }

        if (refresh_flag)
        {
            Current_Page->Page_Item_List[Current_Page->Current_Item].Show_Page_Item();
        }
    }
}
