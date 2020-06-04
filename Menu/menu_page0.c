#include "stdio.h"
#include "menu_core.h"

#define SCREENS_IN_PAGE 5

Menu_Page_t Page0;

static Page_Screen_t Page0_Screen_List[SCREENS_IN_PAGE];

static void Show_Page0_Screen0()
{
    printf("\033[2J\033[1;1H");

    printf("->menu1 screen0\n"
           "  menu1 screen1\n"
           "  menu1 screen2\n"
           "  menu1 screen3\n"
           "  menu1 screen4\n");
}

static void Show_Page0_Screen1()
{
    printf("\033[2J\033[1;1H");

    printf("  menu1 screen0\n"
           "->menu1 screen1\n"
           "  menu1 screen2\n"
           "  menu1 screen3\n"
           "  menu1 screen4\n");
}

static void Show_Page0_Screen2()
{
    printf("\033[2J\033[1;1H");

    printf("  menu1 screen0\n"
           "  menu1 screen1\n"
           "->menu1 screen2\n"
           "  menu1 screen3\n"
           "  menu1 screen4\n");
}

static void Show_Page0_Screen3()
{
    printf("\033[2J\033[1;1H");

    printf("  menu1 screen0\n"
           "  menu1 screen1\n"
           "  menu1 screen2\n"
           "->menu1 screen3\n"
           "  menu1 screen4\n");
}

static void Show_Page0_Screen4()
{
    printf("\033[2J\033[1;1H");

    printf("  menu1 screen0\n"
           "  menu1 screen1\n"
           "  menu1 screen2\n"
           "  menu1 screen3\n"
           "->menu1 screen4\n");
}

static uint8_t Loop_Page0_Screen0(Menu_Event_t *event)
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

static uint8_t Loop_Page0_Screen1(Menu_Event_t *event)
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

static uint8_t Loop_Page0_Screen2(Menu_Event_t *event)
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

static uint8_t Loop_Page0_Screen3(Menu_Event_t *event)
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

static uint8_t Loop_Page0_Screen4(Menu_Event_t *event)
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

    Page0_Screen_List[0].Show_Page_Screen = Show_Page0_Screen0;
    Page0_Screen_List[0].Enter_Page_Screen = Loop_Page0_Screen0;

    Page0_Screen_List[1].Show_Page_Screen = Show_Page0_Screen1;
    Page0_Screen_List[1].Enter_Page_Screen = Loop_Page0_Screen1;

    Page0_Screen_List[2].Show_Page_Screen = Show_Page0_Screen2;
    Page0_Screen_List[2].Enter_Page_Screen = Loop_Page0_Screen2;

    Page0_Screen_List[3].Show_Page_Screen = Show_Page0_Screen3;
    Page0_Screen_List[3].Enter_Page_Screen = Loop_Page0_Screen3;

    Page0_Screen_List[4].Show_Page_Screen = Show_Page0_Screen4;
    Page0_Screen_List[4].Enter_Page_Screen = Loop_Page0_Screen4;

    Page0.Page_Screen_List = Page0_Screen_List;
    Page0.Screens_In_Page = SCREENS_IN_PAGE;
    Menu_Add_Page(&Page0);
}
