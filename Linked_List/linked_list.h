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

#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

/** std includes */
#include <stdint.h>

typedef struct List_Node_t
{
    void *Data;
    struct List_Node_t *Next;

} List_Node_t;

/** list as queue structure */
typedef struct List_Queue_t
{
    List_Node_t *Head;
    List_Node_t *Tail;

} List_Queue_t;

List_Node_t *List_New_Node(void *data);
List_Node_t *List_Find_Node(List_Node_t **head, void *data);

uint8_t List_Add_At_Top(List_Node_t **head, List_Node_t *new_node);
uint8_t List_Add_After(List_Node_t **head, List_Node_t *node, List_Node_t *new_node);
uint8_t List_Add_Before(List_Node_t **head, List_Node_t *node, List_Node_t *new_node);
uint8_t List_Add_At_End(List_Node_t **head, List_Node_t *new_node);
uint8_t List_Delete_From_Top(List_Node_t **head);
uint8_t List_Delete_From_End(List_Node_t **head);
uint8_t List_Delete_Node(List_Node_t **head, List_Node_t *node);
uint8_t List_Delete_All(List_Node_t **head);

uint32_t List_Get_Count(List_Node_t **head);

uint8_t List_Push(List_Node_t **head, void *data);
uint8_t List_Pop(List_Node_t **head, void **data);

uint8_t List_Enqueue(List_Queue_t *q, void *data);
uint8_t List_Dequeue(List_Queue_t *q, void **data);


#endif /* LINKED_LIST_H_ */