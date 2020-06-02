#include "stdio.h"
#include "menu_core.h"
#include "menu_Page2.h"

#define SCREENS_IN_PAGE 3

Menu_Page_t Page2;

Page_Screen_t Screen_List[SCREENS_IN_PAGE];

void Show_Page2_Screen1()
{
}

void Show_Page2_Screen2()
{
}

void Show_Page2_Screen3()
{
}

uint8_t Loop_Page2_Screen1(Menu_Event_t *event)
{
    uint8_t xreturn = 1;

    if (event->Enter_Button_Clicks == 1)
    {
        xreturn = 0; // exit loop
    }

    return xreturn;
}

uint8_t Loop_Page2_Screen2(Menu_Event_t *event)
{
    Menu_Change_Page(2, 1); // change page to page2 screen1
    return 0;               // exit loop
}

uint8_t Loop_Page2_Screen3(Menu_Event_t *event)
{
    uint8_t xreturn = 0;
    return xreturn;
}

void Menu_Page2_Init()
{
    printf("Menu_Page1_Init\n");
    Screen_List[0].Show_Page_Screen = Show_Page2_Screen1;
    Screen_List[0].Enter_Page_Screen = Loop_Page2_Screen1;

    Screen_List[1].Show_Page_Screen = Show_Page2_Screen2;
    Screen_List[1].Enter_Page_Screen = Loop_Page2_Screen2;

    Screen_List[2].Show_Page_Screen = Show_Page2_Screen3;
    Screen_List[2].Enter_Page_Screen = Loop_Page2_Screen3;

    Page2.Page_Screen_List = Screen_List;
    Page2.Screens_In_Page = SCREENS_IN_PAGE;
    Menu_Add_Page(&Page2);
}
