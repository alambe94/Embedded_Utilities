#include "stdio.h"
#include "menu_core.h"

#define ITEMS_IN_PAGE 5

Menu_Page_t Page0;

static Page_Item_t Page0_Item_List[ITEMS_IN_PAGE];

static void Show_Page0_Item0()
{
    printf("\033[2J\033[1;1H");

    printf("->menu1 Item0\n"
           "  menu1 Item1\n"
           "  menu1 Item2\n"
           "  menu1 Item3\n"
           "  menu1 Item4\n");
}

static void Show_Page0_Item1()
{
    printf("\033[2J\033[1;1H");

    printf("  menu1 Item0\n"
           "->menu1 Item1\n"
           "  menu1 Item2\n"
           "  menu1 Item3\n"
           "  menu1 Item4\n");
}

static void Show_Page0_Item2()
{
    printf("\033[2J\033[1;1H");

    printf("  menu1 Item0\n"
           "  menu1 Item1\n"
           "->menu1 Item2\n"
           "  menu1 Item3\n"
           "  menu1 Item4\n");
}

static void Show_Page0_Item3()
{
    printf("\033[2J\033[1;1H");

    printf("  menu1 Item0\n"
           "  menu1 Item1\n"
           "  menu1 Item2\n"
           "->menu1 Item3\n"
           "  menu1 Item4\n");
}

static void Show_Page0_Item4()
{
    printf("\033[2J\033[1;1H");

    printf("  menu1 Item0\n"
           "  menu1 Item1\n"
           "  menu1 Item2\n"
           "  menu1 screen3\n"
           "->menu1 screen4\n");
}

static uint8_t Run_Page0_Item0(Menu_Event_t *event)
{
    uint8_t xreturn = 1;
    static uint8_t count = 0;

    printf("\033[2J\033[1;1H");

    printf("****screen0****\n"
           "*             *\n"
           "*      %d     *\n"
           "*             *\n"
           " **************\n",
           count++);

    if (event->Back_Button_Clicks)
    {
        xreturn = 0;
    }

    return xreturn;
}

static uint8_t Run_Page0_Item1(Menu_Event_t *event)
{
    uint8_t xreturn = 1;
    static uint8_t count = 0;

    printf("\033[2J\033[1;1H");

    printf("****screen1****\n"
           "*             *\n"
           "*      %d     *\n"
           "*             *\n"
           " **************\n",
           count++);

    if (event->Back_Button_Clicks)
    {
        xreturn = 0;
    }

    return xreturn;
}

static uint8_t Run_Page0_Item2(Menu_Event_t *event)
{
    uint8_t xreturn = 1;
    static uint8_t count = 0;

    printf("\033[2J\033[1;1H");

    printf("****screen2****\n"
           "*             *\n"
           "*      %d     *\n"
           "*             *\n"
           " **************\n",
           count++);

    if (event->Back_Button_Clicks)
    {
        xreturn = 0;
    }

    return xreturn;
}

static uint8_t Run_Page0_Item3(Menu_Event_t *event)
{
    uint8_t xreturn = 1;
    static uint8_t count = 0;

    printf("\033[2J\033[1;1H");

    printf("****screen3****\n"
           "*             *\n"
           "*      %d     *\n"
           "*             *\n"
           " **************\n",
           count++);

    if (event->Back_Button_Clicks)
    {
        xreturn = 0;
    }

    return xreturn;
}

static uint8_t Run_Page0_Item4(Menu_Event_t *event)
{
    uint8_t xreturn = 1;
    static uint8_t count = 0;

    printf("\033[2J\033[1;1H");

    printf("****screen4****\n"
           "*             *\n"
           "*      %d     *\n"
           "*             *\n"
           " **************\n",
           count++);

    if (event->Back_Button_Clicks)
    {
        xreturn = 0;
    }

    return xreturn;
}

void Menu_Page0_Init()
{
    printf("Menu_Page0_Init\n");

    Page0_Item_List[0].Show_Page_Item = Show_Page0_Item0;
    Page0_Item_List[0].Page_Item_Callback = Run_Page0_Item0;

    Page0_Item_List[1].Show_Page_Item = Show_Page0_Item1;
    Page0_Item_List[1].Page_Item_Callback = Run_Page0_Item1;

    Page0_Item_List[2].Show_Page_Item = Show_Page0_Item2;
    Page0_Item_List[2].Page_Item_Callback = Run_Page0_Item2;

    Page0_Item_List[3].Show_Page_Item = Show_Page0_Item3;
    Page0_Item_List[3].Page_Item_Callback = Run_Page0_Item3;

    Page0_Item_List[4].Show_Page_Item = Show_Page0_Item4;
    Page0_Item_List[4].Page_Item_Callback = Run_Page0_Item4;

    Page0.Page_Item_List = Page0_Item_List;
    Page0.Items_In_Page = ITEMS_IN_PAGE;
    Menu_Add_Page(&Page0);
}
