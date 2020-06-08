#include "stdio.h"
#include "menu_core.h"

#define ITEMS_IN_PAGE 3

Menu_Page_t Page1;

static Page_Item_t Page1_Item_List[ITEMS_IN_PAGE];

static void Page_Add_Item_Helper(void (*Show)(), uint8_t (*Callback)(Menu_Event_t *event))
{
    static uint8_t Page_Item_Count = 0;
    if (Page_Item_Count < ITEMS_IN_PAGE)
    {
        Page1_Item_List[Page_Item_Count].Show_Page_Item = Show;
        Page1_Item_List[Page_Item_Count].Page_Item_Callback = Callback;
    }
    Page_Item_Count++;
}


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

static uint8_t Run_Page1_Item0(Menu_Event_t *event)
{
    uint8_t xreturn = 1;
    printf("Run_Page1_Item0\n");
    if (event->Back_Button_Clicks)
    {
        xreturn = 0;
    }
    return xreturn;
}

static uint8_t Run_Page1_Item1(Menu_Event_t *event)
{
    uint8_t xreturn = 1;
    printf("Run_Page1_Item1\n");
    if (event->Back_Button_Clicks)
    {
        xreturn = 0;
    }
    return xreturn;
}

static uint8_t Run_Page1_Item2(Menu_Event_t *event)
{
    uint8_t xreturn = 1;
    printf("Run_Page1_Item2\n");
    if (event->Back_Button_Clicks)
    {
        xreturn = 0;
    }
    return xreturn;
}

void Menu_Page1_Init()
{
    printf("Menu_Page1_Init\n");
    Page_Add_Item_Helper(Show_Page1_Item0, Run_Page1_Item0);
    Page_Add_Item_Helper(Show_Page1_Item1, Run_Page1_Item1);
    Page_Add_Item_Helper(Show_Page1_Item2, Run_Page1_Item2);

    Page1.Page_Item_List = Page1_Item_List;
    Page1.Items_In_Page = ITEMS_IN_PAGE;
    Menu_Add_Page(&Page1);
}
