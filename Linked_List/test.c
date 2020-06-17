#include "stdio.h"
#include "stdlib.h"
#include "linked_list.h"

void Test();
void List_As_Stack_Test();
void List_As_Queue_Test();
void List_Assersions();

void main()
{
    //Test();
    //List_As_Stack_Test();
    List_As_Queue_Test();
    //List_Assersions();
}

void Test()
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
    for (List_Node_t *temp = head; temp != NULL; temp = temp->Next)
    {
        printf("%s", temp->Data);
    }
    printf("\r\n");

    List_Add_At_Top(&head, List_New_Node("top\n"));
    List_Add_At_Top(&head, List_New_Node("top\n"));
    List_Add_At_Top(&head, List_New_Node("top\n"));
    List_Add_At_Top(&head, List_New_Node("top\n"));
    List_Add_At_Top(&head, List_New_Node("top\n"));
    List_Add_At_Top(&head, List_New_Node("top\n"));
    List_Add_At_Top(&head, List_New_Node("top\n"));
    List_Add_At_Top(&head, List_New_Node("top\n"));
    List_Add_At_Top(&head, List_New_Node("top\n"));
    List_Add_At_Top(&head, List_New_Node("top\n"));
    for (List_Node_t *temp = head; temp != NULL; temp = temp->Next)
    {
        printf("%s", temp->Data);
    }
    printf("\r\n");

    List_Add_At_End(&head, List_New_Node("bottom\n"));
    List_Add_At_End(&head, List_New_Node("bottom\n"));
    List_Add_At_End(&head, List_New_Node("bottom\n"));
    List_Add_At_End(&head, List_New_Node("bottom\n"));
    List_Add_At_End(&head, List_New_Node("bottom\n"));
    List_Add_At_End(&head, List_New_Node("bottom\n"));
    List_Add_At_End(&head, List_New_Node("bottom\n"));
    List_Add_At_End(&head, List_New_Node("bottom\n"));
    List_Add_At_End(&head, List_New_Node("bottom\n"));
    List_Add_At_End(&head, List_New_Node("bottom\n"));
    List_Add_At_End(&head, List_New_Node("bottom\n"));
    List_Add_At_End(&head, List_New_Node("bottom\n"));
    List_Add_At_End(&head, List_New_Node("bottom\n"));
    List_Add_At_End(&head, List_New_Node("bottom\n"));
    for (List_Node_t *temp = head; temp != NULL; temp = temp->Next)
    {
        printf("%s", temp->Data);
    }
    printf("\r\n");

    printf("list find = %s", List_Find_Node(&head, first->Data)->Data);
    printf("list find = %s", List_Find_Node(&head, second->Data)->Data);
    printf("list find = %s", List_Find_Node(&head, third->Data)->Data);
    printf("list find = %s", List_Find_Node(&head, fourth->Data)->Data);

    List_Delete_From_End(&head);
    List_Delete_From_End(&head);
    List_Delete_From_End(&head);
    List_Delete_From_End(&head);
    List_Delete_From_End(&head);
    for (List_Node_t *temp = head; temp != NULL; temp = temp->Next)
    {
        printf("%s", temp->Data);
    }
    printf("\r\n");

    List_Delete_From_Top(&head);
    List_Delete_From_Top(&head);
    List_Delete_From_Top(&head);
    List_Delete_From_Top(&head);
    List_Delete_From_Top(&head);
    List_Delete_From_Top(&head);
    List_Delete_From_Top(&head);
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

void List_As_Stack_Test()
{
    List_Node_t *head = NULL;
    void *data;

    List_Push(&head, "1");
    List_Push(&head, "2");
    List_Push(&head, "3");
    List_Push(&head, "4");
    List_Push(&head, "5");
    List_Push(&head, "6");
    List_Push(&head, "7");
    List_Push(&head, "8");
    List_Push(&head, "9");
    List_Push(&head, "10");
    List_Push(&head, "11");
    List_Push(&head, "12");
    List_Push(&head, "13");
    List_Push(&head, "14");
    List_Push(&head, "15");
    List_Push(&head, "16");
    List_Push(&head, "17");
    List_Push(&head, "18");
    List_Push(&head, "19");
    List_Push(&head, "20");

    /* data on heap */
    for (uint16_t i = 21; i < 50000; i++)
    {
        char *buf = malloc(6);
        snprintf(buf, 6, "%d", i);
        List_Push(&head, buf);
    }

    while (List_Pop(&head, &data))
    {
        printf("%s\n", data);
    }
}

void List_As_Queue_Test()
{
    List_Queue_t Queue = {.Head = NULL, .Tail = NULL};
    void *data;

    List_Enqueue(&Queue, "1");
    List_Enqueue(&Queue, "2");
    List_Enqueue(&Queue, "3");
    List_Enqueue(&Queue, "4");
    List_Enqueue(&Queue, "5");
    List_Enqueue(&Queue, "6");
    List_Enqueue(&Queue, "7");
    List_Enqueue(&Queue, "8");
    List_Enqueue(&Queue, "9");
    List_Enqueue(&Queue, "10");
    List_Enqueue(&Queue, "11");
    List_Enqueue(&Queue, "12");
    List_Enqueue(&Queue, "13");
    List_Enqueue(&Queue, "14");
    List_Enqueue(&Queue, "15");
    List_Enqueue(&Queue, "16");
    List_Enqueue(&Queue, "17");
    List_Enqueue(&Queue, "18");
    List_Enqueue(&Queue, "19");
    List_Enqueue(&Queue, "20");

    for (uint16_t i = 21; i < 50000; i++)
    {
        char *buf = malloc(6);
        snprintf(buf, 6, "%d", i);
        List_Enqueue(&Queue, buf);
    }

    while (List_Dequeue(&Queue, &data))
    {
        printf("%s\n", data);
    }
}

void List_Assersions()
{
    List_New_Node(NULL);
    List_Add_At_Top(NULL, NULL);
    List_Add_After(NULL, NULL, NULL);
}