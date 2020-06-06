#ifndef INC_MENU_CORE_H_
#define INC_MENU_CORE_H_

#include "stdint.h"

#define USE_MENU_ASSERT 1

typedef struct Menu_Event_t
{
    int16_t Encoder_Count;       // encoder value if any
    uint8_t Enter_Button_Clicks; // enter or select button

    uint8_t Up_Button_Clicks;    // if any
    uint8_t Down_Button_Clicks;  // if any
    uint8_t Left_Button_Clicks;  // if any
    uint8_t Right_Button_Clicks; // if any
    uint8_t Back_Button_Clicks;  // if any

} Menu_Event_t;

typedef struct Page_Item_t
{
    void (*Show_Page_Item)();
    uint8_t (*Page_Item_Callback)(Menu_Event_t *event);

} Page_Item_t;

typedef struct Menu_Page_t
{
    Page_Item_t *Page_Item_List;
    uint8_t Items_In_Page;
    uint8_t Current_Item;

} Menu_Page_t;

void Menu_Loop();
uint8_t Menu_Add_Page(Menu_Page_t *page);
void Menu_Change_Page(uint8_t page_no, uint8_t page_Item);

#endif /* INC_MENU_CORE_H_ */
