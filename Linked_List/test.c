

#include "stdio.h"
#include "linked_list.h"

void main()
{
    List_Node_t *head = NULL;

    List_Add(&head, List_New_Node("first node\n"));
    List_Add(&head, List_New_Node("second node\n"));
    List_Add(&head, List_New_Node("third node\n"));
    List_Add(&head, List_New_Node("fourth node\n"));

    //for(List_Node_t *temp = head; head->Next!=NULL; head=head->Next)
    //{
        //printf("%s", temp->Data);
    //}
}