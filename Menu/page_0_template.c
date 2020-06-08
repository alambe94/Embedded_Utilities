#include "stdio.h"
#include "menu_core.h"

#define ITEMS_IN_PAGE 5

Menu_Page_t Page0;

static Page_Item_t Page0_Item_List[ITEMS_IN_PAGE];

static void Page_Add_Item_Helper(void (*Show)(), uint8_t (*Callback)(Menu_Event_t *event))
{
    static uint8_t Page_Item_Count = 0;
    if (Page_Item_Count < ITEMS_IN_PAGE)
    {
        Page0_Item_List[Page_Item_Count].Show_Page_Item = Show;
        Page0_Item_List[Page_Item_Count].Page_Item_Callback = Callback;
    }
    Page_Item_Count++;
}

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

    Page_Add_Item_Helper(Show_Page0_Item0, Run_Page0_Item0);
    Page_Add_Item_Helper(Show_Page0_Item1, Run_Page0_Item1);
    Page_Add_Item_Helper(Show_Page0_Item2, Run_Page0_Item2);
    Page_Add_Item_Helper(Show_Page0_Item3, Run_Page0_Item3);
    Page_Add_Item_Helper(Show_Page0_Item4, Run_Page0_Item4);

    Page0.Page_Item_List = Page0_Item_List;
    Page0.Items_In_Page = ITEMS_IN_PAGE;
    Menu_Add_Page(&Page0);
}
