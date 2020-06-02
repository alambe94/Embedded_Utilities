#include "stdio.h"
#include "menu_core.h"
#include "menu_page1.h"
#include "menu_page2.h"

#define MAX_PAGES 10
#define REFRESH_CYCLE 20

static uint8_t Refresh_Flag;

static Menu_Page_t *Menu_Page_List[MAX_PAGES];

Menu_Page_t *Current_Page;

extern uint64_t Menu_Get_Tick();

extern void Menu_Get_Event(Menu_Event_t *event);

void Menu_Add_Page(Menu_Page_t *page)
{
	static uint8_t Menu_Page_Count;

	if (Menu_Page_Count < MAX_PAGES)
	{
		Menu_Page_List[Menu_Page_Count++] = page;
	}
}

void Menu_Change_Page(uint8_t page_no, uint8_t page_screen)
{
	if (page_no < MAX_PAGES)
	{
		Current_Page = Menu_Page_List[page_no];

		if (page_screen == 0)
		{
			page_screen = 1;
		}

		Current_Page->Current_Screen = page_screen;

		Refresh_Flag = 1;
	}
}

void Menu_Init()
{
	Menu_Page1_Init();
	Menu_Page2_Init();
	Menu_Change_Page(1, 1); // by default show page1 screen1.
	Refresh_Flag = 0;
}

void Menu_Loop()
{
	static uint8_t in_page_loop = 1; // by default enter page1 screen1 (Home Screen).

	static uint64_t Scan_Time_Stamp = 0;

	Menu_Event_t menu_event;

	if (Menu_Get_Tick() - Scan_Time_Stamp > (REFRESH_CYCLE - 1))
	{
		Scan_Time_Stamp = Menu_Get_Tick();

		printf("Menu_Loop\n");

		Menu_Get_Event(&menu_event);

		if (!in_page_loop)
		{
			printf("in_page_loop\n");

			if (menu_event.Encoder_Count < 0) // or down button
			{
				Current_Page->Current_Screen++;
				if (Current_Page->Current_Screen++ > Current_Page->Screens_In_Page)
				{
					Current_Page->Current_Screen = Current_Page->Screens_In_Page;
				}
				Current_Page->Page_Screen_List[Current_Page->Current_Screen].Show_Page_Screen();
			}

			if (menu_event.Encoder_Count > 0) // or up button
			{
				Current_Page->Current_Screen--;
				if (Current_Page->Current_Screen == 0)
				{
					Current_Page->Current_Screen = 1;
				}
				Current_Page->Page_Screen_List[Current_Page->Current_Screen].Show_Page_Screen();
			}

			if (menu_event.Enter_Button_Clicks == 1) // enter or select button
			{
				menu_event.Enter_Button_Clicks = 0; // click and count does not belong to page loop so reset them.
				menu_event.Encoder_Count = 0;
				in_page_loop = Current_Page->Page_Screen_List[Current_Page->Current_Screen].Enter_Page_Screen(&menu_event);
			}

			if (Refresh_Flag == 1)
			{
				printf("Refresh_Flag\n");
				Refresh_Flag = 0;
				Current_Page->Page_Screen_List[Current_Page->Current_Screen].Show_Page_Screen();
			}
		}
		else
		{
			printf("!in_page_loop\n");

			in_page_loop = Current_Page->Page_Screen_List[1].Enter_Page_Screen(&menu_event);

			printf("in_page_loop = Current_Page->Page_Screen_List\n");

			if (!in_page_loop)
			{
				Refresh_Flag = 1;
			}
		}
	}
}
