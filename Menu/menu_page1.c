#include "stdio.h"
#include "menu_core.h"

#define SCREENS_IN_PAGE 3

Menu_Page_t Page1;

static Page_Screen_t Page1_Screen_List[SCREENS_IN_PAGE];

static void Show_Page1_Screen0()
{
    printf("Show_Page1_Screen0\n");
}

static void Show_Page1_Screen1()
{
    printf("Show_Page1_Screen1\n");
}

static void Show_Page1_Screen2()
{
    printf("Show_Page1_Screen2\n");
}

static uint8_t Loop_Page1_Screen0(Menu_Event_t *event)
{
    uint8_t xreturn = 1;
    printf("Loop_Page1_Screen0\n");
    if (event->Back_Button_Clicks)
    {
        xreturn = 0;
    }
    return xreturn;
}

static uint8_t Loop_Page1_Screen1(Menu_Event_t *event)
{
    uint8_t xreturn = 1;
    printf("Loop_Page1_Screen1\n");
    if (event->Back_Button_Clicks)
    {
        xreturn = 0;
    }
    return xreturn;
}

static uint8_t Loop_Page1_Screen2(Menu_Event_t *event)
{
    uint8_t xreturn = 1;
    printf("Loop_Page1_Screen2\n");
    if (event->Back_Button_Clicks)
    {
        xreturn = 0;
    }
    return xreturn;
}

void Menu_Page1_Init()
{
    printf("Menu_Page1_Init\n");
    Page1_Screen_List[0].Show_Page_Screen = Show_Page1_Screen0;
    Page1_Screen_List[0].Enter_Page_Screen = Loop_Page1_Screen0;

    Page1_Screen_List[1].Show_Page_Screen = Show_Page1_Screen1;
    Page1_Screen_List[1].Enter_Page_Screen = Loop_Page1_Screen1;

    Page1_Screen_List[2].Show_Page_Screen = Show_Page1_Screen2;
    Page1_Screen_List[2].Enter_Page_Screen = Loop_Page1_Screen2;

    Page1.Page_Screen_List = Page1_Screen_List;
    Page1.Screens_In_Page = SCREENS_IN_PAGE;
    Menu_Add_Page(&Page1);
}
