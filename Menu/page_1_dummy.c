#include "stdio.h"
#include "menu_core.h"

#define ITEMS_IN_PAGE 3

Menu_Page_t Page1;

static Page_Item_t Page1_Item_List[ITEMS_IN_PAGE];

static void Show_Page1_Item0()
{
    printf("Show_Page1_Item0\n");
}

static void Show_Page1_Item1()
{
    printf("Show_Page1_Item1\n");
}

static void Show_Page1_Item2()
{
    printf("Show_Page1_Item2\n");
}

static uint8_t Loop_Page1_Item0(Menu_Event_t *event)
{
    uint8_t xreturn = 1;
    printf("Loop_Page1_Item0\n");
    if (event->Back_Button_Clicks)
    {
        xreturn = 0;
    }
    return xreturn;
}

static uint8_t Loop_Page1_Item1(Menu_Event_t *event)
{
    uint8_t xreturn = 1;
    printf("Loop_Page1_Item1\n");
    if (event->Back_Button_Clicks)
    {
        xreturn = 0;
    }
    return xreturn;
}

static uint8_t Loop_Page1_Item2(Menu_Event_t *event)
{
    uint8_t xreturn = 1;
    printf("Loop_Page1_Item2\n");
    if (event->Back_Button_Clicks)
    {
        xreturn = 0;
    }
    return xreturn;
}

void Menu_Page1_Init()
{
    printf("Menu_Page1_Init\n");
    Page1_Item_List[0].Show_Page_Item = Show_Page1_Item0;
    Page1_Item_List[0].Page_Item_Callback = Loop_Page1_Item0;

    Page1_Item_List[1].Show_Page_Item = Show_Page1_Item1;
    Page1_Item_List[1].Page_Item_Callback = Loop_Page1_Item1;

    Page1_Item_List[2].Show_Page_Item = Show_Page1_Item2;
    Page1_Item_List[2].Page_Item_Callback = Loop_Page1_Item2;

    Page1.Page_Item_List = Page1_Item_List;
    Page1.Items_In_Page = ITEMS_IN_PAGE;
    Menu_Add_Page(&Page1);
}
