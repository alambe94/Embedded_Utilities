#include "stdio.h"
#include "menu_core.h"
#include "menu_page1.h"

#define SCREENS_IN_PAGE 5

Menu_Page_t Page1;

Page_Screen_t Screen_List[SCREENS_IN_PAGE];

void Show_Page1_Screen1()
{
    printf("\033[6;3H");
    printf("->menu1 screen1\n"
           "  menu1 screen2\n"
           "  menu1 screen3\n"
           "  menu1 screen4\n"
           "  menu1 screen5\n");
}

void Show_Page1_Screen2()
{
    printf("\033[6;3H");
    printf("  menu1 screen1\n"
           "->menu1 screen2\n"
           "  menu1 screen3\n"
           "  menu1 screen4\n"
           "  menu1 screen5\n");
}

void Show_Page1_Screen3()
{
    printf("\033[6;3H");
    printf("  menu1 screen1\n"
           "  menu1 screen2\n"
           "->menu1 screen3\n"
           "  menu1 screen4\n"
           "  menu1 screen5\n");
}

void Show_Page1_Screen4()
{
    printf("\033[6;3H");
    printf("  menu1 screen1\n"
           "  menu1 screen2\n"
           "  menu1 screen3\n"
           "->menu1 screen4\n"
           "  menu1 screen5\n");
}

void Show_Page1_Screen5()
{
    printf("\033[6;3H");
    printf("  menu1 screen1\n"
           "  menu1 screen2\n"
           "  menu1 screen3\n"
           "  menu1 screen4\n"
           "->menu1 screen5\n");
}

uint8_t Loop_Page1_Screen1(Menu_Event_t *event)
{
    uint8_t xreturn = 1;
    uint8_t count = 0;

    if (event->Enter_Button_Clicks == 1)
    {
        xreturn = 0; // exit loop
    }

    printf("\033[6;3H");
    printf("****screen1****\n"
           "*             *\n"
           "*      %d     *\n"
           "*             *\n"
           " **************\n", count++);

    return xreturn;
}

uint8_t Loop_Page1_Screen2(Menu_Event_t *event)
{
    Menu_Change_Page(2, 1); // change page to page2 screen1
    return 0;               // exit loop
}

uint8_t Loop_Page1_Screen3(Menu_Event_t *event)
{
    uint8_t xreturn = 0;
    return xreturn;
}

uint8_t Loop_Page1_Screen4(Menu_Event_t *event)
{
    uint8_t xreturn = 0;

    return xreturn;
}

uint8_t Loop_Page1_Screen5(Menu_Event_t *event)
{
    uint8_t xreturn = 0;

    return xreturn;
}

void Menu_Page1_Init()
{
    printf("Menu_Page1_Init\n");

    Screen_List[0].Show_Page_Screen = Show_Page1_Screen1;
    Screen_List[0].Enter_Page_Screen = Loop_Page1_Screen1;

    Screen_List[1].Show_Page_Screen = Show_Page1_Screen2;
    Screen_List[1].Enter_Page_Screen = Loop_Page1_Screen2;

    Screen_List[2].Show_Page_Screen = Show_Page1_Screen3;
    Screen_List[2].Enter_Page_Screen = Loop_Page1_Screen3;

    Screen_List[3].Show_Page_Screen = Show_Page1_Screen4;
    Screen_List[3].Enter_Page_Screen = Loop_Page1_Screen4;

    Screen_List[4].Show_Page_Screen = Show_Page1_Screen5;
    Screen_List[4].Enter_Page_Screen = Loop_Page1_Screen5;

    Page1.Page_Screen_List = Screen_List;
    Page1.Screens_In_Page = SCREENS_IN_PAGE;
    Menu_Add_Page(&Page1);
}
