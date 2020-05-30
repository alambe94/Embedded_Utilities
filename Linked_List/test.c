

#include "stdio.h"
#include "linked_list.h"

void main()
{
    List_Node_t *head = NULL;

    List_Node_t *first = List_New_Node("first node\n");
    List_Node_t *second = List_New_Node("second node\n");
    List_Node_t *third = List_New_Node("third node\n");
    List_Node_t *fourth = List_New_Node("fourth node\n");

    List_Add_At_End(&head, first);
    List_Add_At_End(&head, second);
    List_Add_Before(&head, second, third);
    List_Add_After(&head, first, fourth);

    printf("list find = %s", List_Find_Node(&head, first->Data)->Data);
    printf("list find = %s", List_Find_Node(&head, second->Data)->Data);
    printf("list find = %s", List_Find_Node(&head, third->Data)->Data);
    printf("list find = %s", List_Find_Node(&head, fourth->Data)->Data);

    printf("\r\n");

    for (List_Node_t *temp = head; temp != NULL; temp = temp->Next)
    {
        printf("%s", temp->Data);
    }
    printf("\r\n");

    List_Delete_From_End(&head);

    for (List_Node_t *temp = head; temp != NULL; temp = temp->Next)
    {
        printf("%s", temp->Data);
    }
    printf("\r\n");

    List_Delete(&head);

    for (List_Node_t *temp = head; temp != NULL; temp = temp->Next)
    {
        printf("%s", temp->Data);
    }
    printf("\r\n");

    List_Delete_Node(&head, third);

    for (List_Node_t *temp = head; temp != NULL; temp = temp->Next)
    {
        printf("%s", temp->Data);
    }
    printf("\r\n");
}