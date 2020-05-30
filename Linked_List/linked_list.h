#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include "stdint.h"

typedef struct List_Node_t
{
    void *Data;
    struct List_Node_t *Next;
} List_Node_t;

List_Node_t *List_New_Node(void *data);
List_Node_t *List_Find_Node(List_Node_t **head, void *data);

uint8_t List_Add(List_Node_t **head, List_Node_t *new_node);
uint8_t List_Add_After(List_Node_t **head, List_Node_t *node, List_Node_t *new_node);
uint8_t List_Add_Before(List_Node_t **head, List_Node_t *node, List_Node_t *new_node);
uint8_t List_Add_At_End(List_Node_t **head, List_Node_t *new_node);
uint8_t List_Delete(List_Node_t **head);
uint8_t List_Delete_From_End(List_Node_t **head);
uint8_t List_Delete_Node(List_Node_t **head, List_Node_t *node);


#endif /* LINKED_LIST_H_ */