
#include "linked_list.h"
#include "stdlib.h"
#include "stdio.h"

List_Node_t *List_New_Node(void *data)
{
    List_Node_t *new_node = (List_Node_t *)malloc(sizeof(List_Node_t));
    new_node->Data = data;
    new_node->Next = NULL;
    return new_node;
}
/**
 *
 **/
uint8_t List_Add(List_Node_t **head, List_Node_t *new_node)
{
    uint8_t xreturn = 0;

    /* this is first node in the list */
    if (head == NULL && new_node != NULL)
    {
        printf("%s", new_node->Data);
        *head = new_node;
        xreturn = 1;
    }
    else if (*head != NULL && new_node != NULL)
    {
        printf("%s", new_node->Data);
        new_node->Next = *head;

        *head = new_node;

        xreturn = 1;
    }

    return xreturn;
}

uint8_t List_Add_After(List_Node_t *node, List_Node_t *new_node)
{
    uint8_t xreturn = 0;

    if (node != NULL && new_node != NULL)
    {
        new_node->Next = node->Next;

        node->Next = new_node;

        xreturn = 1;
    }

    return xreturn;
}

uint8_t List_Add_Before(List_Node_t **head, List_Node_t *node, List_Node_t *new_node)
{
    uint8_t xreturn = 0;

    if (head != NULL && *head != NULL && node != NULL && new_node != NULL)
    {
        List_Node_t *temp_node = *head;

        while (temp_node->Next != NULL && temp_node->Next != node)
        {
            temp_node = temp_node->Next;
        }

        temp_node->Next = new_node;

        new_node->Next = node;

        xreturn = 1;
    }

    return xreturn;
}

uint8_t List_Add_At_End(List_Node_t **head, List_Node_t *new_node)
{
    uint8_t xreturn = 0;

    if (head != NULL && *head != NULL && new_node != NULL)
    {
        List_Node_t *last = *head;

        while (last->Next != NULL)
        {
            last = last->Next;
        }

        last->Next = new_node;

        xreturn = 1;
    }

    return xreturn;
}

uint8_t List_Delete(List_Node_t **head)
{
    uint8_t xreturn = 0;

    if (head != NULL && *head != NULL)
    {
        xreturn = 1;

        /* check if it is only node in list*/
        if ((*head)->Next == NULL)
        {
            free(*head);
            *head = NULL;
        }
        else
        {
            List_Node_t *temp = *head;
            *head = (*head)->Next;
            free(temp);
        }
    }

    return xreturn;
}

uint8_t List_Delete_From_End(List_Node_t **head)
{
    uint8_t xreturn = 0;

    if (head != NULL && *head != NULL)
    {
        List_Node_t *second_last = *head;
        xreturn = 1;

        /* check if it is only node in list*/
        if ((*head)->Next == NULL)
        {
            free(head);

            *head = NULL;
        }
        else
        {
            while (second_last->Next->Next != NULL)
            {
                second_last = second_last->Next;
            }

            free(second_last->Next);

            second_last->Next = NULL;
        }
    }

    return xreturn;
}

uint8_t List_Delete_Node(List_Node_t **head, List_Node_t *node)
{
    uint8_t xreturn = 0;

    if (head != NULL && *head != NULL && node != NULL)
    {
        List_Node_t *temp_node = *head;

        while (temp_node->Next != NULL && temp_node->Next != node)
        {
            temp_node = temp_node->Next;
        }

        temp_node->Next = node->Next;

        free(node);

        xreturn = 1;
    }

    return xreturn;
}

List_Node_t *List_Find_Node(List_Node_t **head, void *data)
{
    if (head != NULL && *head != NULL)
    {
        List_Node_t *temp_node = *head;

        while (temp_node != NULL && temp_node->Data != data)
        {
            temp_node = temp_node->Next;
        }

        return temp_node;
    }

    return NULL;
}