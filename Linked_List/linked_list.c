
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
uint8_t List_Add_At_Top(List_Node_t **head, List_Node_t *new_node)
{
    uint8_t xreturn = 0;

    if (head != NULL && new_node != NULL)
    {
        xreturn = 1;
        new_node->Next = *head;
        *head = new_node;
    }

    return xreturn;
}

uint8_t List_Add_After(List_Node_t **head, List_Node_t *node, List_Node_t *new_node)
{
    uint8_t xreturn = 0;

    if (head != NULL && *head != NULL && node != NULL && new_node != NULL)
    {
        xreturn = 1;
        new_node->Next = node->Next;
        node->Next = new_node;
    }

    return xreturn;
}

uint8_t List_Add_Before(List_Node_t **head, List_Node_t *node, List_Node_t *new_node)
{
    uint8_t xreturn = 0;

    if (head != NULL && *head != NULL && node != NULL && new_node != NULL)
    {
        /* check if it is the fist node */
        if (*head == node)
        {
            xreturn = 1;
            new_node->Next = *head;
            *head = new_node;
        }
        else
        {
            List_Node_t *temp_node = *head;

            while (temp_node != NULL && temp_node->Next != node)
            {
                temp_node = temp_node->Next;
            }

            if (temp_node != NULL)
            {
                xreturn = 1;
                temp_node->Next = new_node;
                new_node->Next = node;
            }
        }
    }

    return xreturn;
}

uint8_t List_Add_At_End(List_Node_t **head, List_Node_t *new_node)
{
    uint8_t xreturn = 0;

    if (head != NULL && new_node != NULL)
    {
        xreturn = 1;

        /* if this is the first node in list*/
        if (*head == NULL)
        {
            *head = new_node;
        }
        else
        {
            List_Node_t *last = *head;

            while (last->Next != NULL)
            {
                last = last->Next;
            }

            last->Next = new_node;
        }
    }

    return xreturn;
}

uint8_t List_Delete_From_Top(List_Node_t **head)
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
        xreturn = 1;

        /* check if it is only node in list*/
        if ((*head)->Next == NULL)
        {
            free(head);
            *head = NULL;
        }
        else
        {
            List_Node_t *temp_node = *head;

            while (temp_node->Next->Next != NULL)
            {
                temp_node = temp_node->Next;
            }

            free(temp_node->Next);
            temp_node->Next = NULL;
        }
    }

    return xreturn;
}

uint8_t List_Delete_Node(List_Node_t **head, List_Node_t *node)
{
    uint8_t xreturn = 0;

    if (head != NULL && *head != NULL && node != NULL)
    {
        /* check if it is the fist node */
        if (*head == node)
        {
            xreturn = 1;
            free(node);
            *head = node->Next;
        }
        else
        {
            List_Node_t *temp_node = *head;

            while (temp_node != NULL && temp_node->Next != node)
            {
                temp_node = temp_node->Next;
            }

            if (temp_node != NULL)
            {
                xreturn = 1;
                temp_node->Next = node->Next;
                free(node);
            }
        }
    }

    return xreturn;
}

uint8_t List_Delete_All(List_Node_t **head)
{
    uint8_t xreturn = 0;

    if (head != NULL && *head != NULL)
    {
        xreturn = 1;
        List_Node_t *next = NULL;
        List_Node_t *temp = *head;

        while (temp != NULL)
        {
            next = temp->Next;
            free(temp);
            temp = next;
        }
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

        if (temp_node != NULL)
        {
            return temp_node;
        }
    }

    return NULL;
}

uint8_t List_Push(List_Node_t **head, void *data)
{
    return List_Add_At_Top(head, List_New_Node(data));
}

uint8_t List_Pop(List_Node_t **head, void **data)
{
    uint8_t xreturn = 0;

    if (head != NULL && *head != NULL)
    {
        *data = (*head)->Data;
    }

    return List_Delete_From_Top(head);
}

uint16_t List_Get_Count(List_Node_t **head)
{
    uint16_t count = 0;

    for (List_Node_t *temp = *head; temp != NULL; temp = temp->Next)
    {
        count++;
    }

    return count;
}