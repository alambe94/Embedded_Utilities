#include "stdio.h"
#include "menu_core.h"

/**
 * @brief assert implemenation, set MENU_USE_ASSERT to 1 to enable assert
 **/
#if (MENU_USE_ASSERT == 1)
#include "stdio.h"
#define MENU_ASSERT(expr, msg) ((expr) ? (void)0U : Menu_Assert(msg, "menu_core.c", __LINE__))
static void Menu_Assert(char *msg, char *file, uint32_t line)
{
    printf("%s, assertion failed, file=%s, line=%lu\n", msg, file, line);
}
#else
#define MENU_ASSERT(expr, msg) ((void)0U)
#endif

/**
 * @brief list of all registered pages
 **/
static Menu_Page_t *Menu_Page_List[MENU_MAX_PAGES];

/**
 * @brief count of registred pages
 **/
static uint8_t Menu_Page_Count = 0;

/**
 * @brief handle of current or active page
 **/
Menu_Page_t *Current_Page;

/**
 * @brief need to define by user externally. used for timing purpose. simply return millis or tick elapsed
 **/
extern uint32_t Menu_Get_Tick();

/**
 * @brief need to define by user externally. used for navigation purpose. return state of navigation keys
 **/
extern void Menu_Get_Event(Menu_Event_t *event);

/**
 * @brief add given page to list of registered pages
 * @param handle handle of page to be registered
 * @retval return page ID (index+1  of page in registred list), return 0 on failure
 * @note adjust MENU_MAX_PAGES accordingly
 **/
uint8_t Menu_Add_Page(Menu_Page_t *page)
{
    MENU_ASSERT(page, "NULL Passed");
    MENU_ASSERT(Menu_Page_Count < MENU_MAX_PAGES, "MAX Page count reached");

    uint8_t page_valid_flag = 1;

    if (Menu_Page_Count < MENU_MAX_PAGES && page != NULL)
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
                    page_valid_flag = 0;
                }
            }
        }
        else
        {
            page_valid_flag = 0;
        }
    }
    else
    {
        page_valid_flag = 0;
    }

    if (page_valid_flag)
    {
        Menu_Page_List[Menu_Page_Count++] = page;
        /** return page ID */
        return Menu_Page_Count;
    }
    /** return error */
    return 0;
}

/**
 * @brief change current menu and screen(item) to specified page
 * @param page_no page number to switch to
 * @param page_Item page item to switch to
 * @retval return 1 if success
 * @note adjust MENU_MAX_PAGES accordingly
 **/
uint8_t Menu_Change_Page(uint8_t page_no, uint8_t page_Item)
{
    MENU_ASSERT(page_no < MENU_MAX_PAGES, "Page out of index");

    uint8_t ok_flag = 1;

    if (page_no < MENU_MAX_PAGES)
    {
        Current_Page = Menu_Page_List[page_no];

        MENU_ASSERT(page_Item < Current_Page->Items_In_Page, "Item out of index");

        if (page_Item < Current_Page->Items_In_Page)
        {
            Current_Page->Current_Item = page_Item;
        }
        else
        {
            ok_flag = 0;
        }
    }
    else
    {
        ok_flag = 0;
    }
    
    return ok_flag;
}

/**
 * @brief frequently called in main loop. should be called at least every 20ms?
 * @param none
 * @retval none
 **/
void Menu_Loop()
{
    static uint32_t Scan_Time_Stamp = 0;

    if (Menu_Get_Tick() - Scan_Time_Stamp > (MENU_REFRESH_TICK - 1))
    {
        /** by default item_callback_flag set to 1 to call Home Screen*/
        /** call Menu_Change_Page(HOME_PAGE, HOME_ITEM) in init to set home page and screen */
        static uint8_t item_callback_flag = 1;
        uint8_t item_show_flag = 0;
        Menu_Event_t menu_event;

        Scan_Time_Stamp = Menu_Get_Tick();
        Menu_Get_Event(&menu_event);

        if (item_callback_flag)
        {
            item_callback_flag = Current_Page->Page_Item_List[Current_Page->Current_Item].Page_Item_Callback(&menu_event);
            item_show_flag = !item_callback_flag;
        }
        else
        {
            /** enter button or select button is pressed */
            if (menu_event.Enter_Button_Clicks == 1)
            {
                item_callback_flag = 1;
            }
            /** up is pressed or encoder incremented */
            /** down is pressed or encoder decremented */
            else if (menu_event.Encoder_Count)
            {
                int16_t temp = Current_Page->Current_Item + menu_event.Encoder_Count;
                if (temp < 0)
                {
                    temp = 0;
                }
                else if (temp >= Current_Page->Items_In_Page)
                {
                    temp = Current_Page->Items_In_Page - 1;
                }
                Current_Page->Current_Item = temp;
                item_show_flag = 1;
            }
        }

        if (item_show_flag)
        {
            Current_Page->Page_Item_List[Current_Page->Current_Item].Show_Page_Item();
        }
    }
}
