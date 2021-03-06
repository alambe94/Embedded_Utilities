/*
 * file version V0.0.0
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef INC_MENU_CORE_H_
#define INC_MENU_CORE_H_

/** std includes */
#include <stdint.h>

/** if up, down, left, right navigation keys are used else use encoder navigation */
#define MENU_USE_NAVIGATION_KEYS 0

typedef struct Menu_Event_t
{
    /** enter or select button */
    uint8_t Enter_Button_Clicks;

    uint8_t Back_Button_Clicks;

    /** navigation keys if any */
    uint8_t Up_Button_Clicks;
    uint8_t Down_Button_Clicks;
    uint8_t Left_Button_Clicks;
    uint8_t Right_Button_Clicks;

    /** encoder value if any */
    int16_t Encoder_Count;

} Menu_Event_t;

typedef struct Page_Item_t
{
    void (*Show_Page_Item)(void);
    uint8_t (*Page_Item_Callback)(Menu_Event_t *event);

} Page_Item_t;

typedef struct Menu_Page_t
{
    Page_Item_t *Page_Item_List;
    uint8_t Items_In_Page;
    uint8_t Current_Item;

} Menu_Page_t;

void Menu_Loop(void);
int32_t Menu_Add_Page(Menu_Page_t *page);
uint8_t Menu_Change_Page(uint8_t page_no, uint8_t page_Item);

#endif /* INC_MENU_CORE_H_ */
