

#include "stdio.h"
#include "linked_list.h"

void main()
{
    List_Node_t *head = NULL;

    List_Add_At_End(&head, List_New_Node("first node\n"));
    List_Add_At_End(&head, List_New_Node("second node\n"));
    List_Add_At_End(&head, List_New_Node("third node\n"));
    List_Add_At_End(&head, List_New_Node("fourth node\n"));
    List_Add_At_End(&head, List_New_Node("fifth node\n"));
    List_Add_At_End(&head, List_New_Node("sixth node\n"));
    List_Add_At_End(&head, List_New_Node("seventh node\n"));
    List_Add_At_End(&head, List_New_Node("eigth node\n"));
    List_Add_At_End(&head, List_New_Node("ninth node\n"));
    List_Add_At_End(&head, List_New_Node("tenth node\n"));

    for (List_Node_t *temp = head; temp != NULL; temp = temp->Next)
    {
        printf("%s", temp->Data);
    }
    printf("\r\n");

    List_Delete_From_End(&head);
    List_Delete_From_End(&head);


    for (List_Node_t *temp = head; temp != NULL; temp = temp->Next)
    {
        printf("%s", temp->Data);
    }
}