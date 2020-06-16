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

/** std includes */
#include <stdlib.h>

/** list includes */
#include "linked_list.h"

/** enable/disable assert */
#define USE_LIST_ASSERT 1

#if (USE_LIST_ASSERT == 1)
#include <stdio.h>
#define LIST_ASSERT(expr, msg) ((expr) ? (void)0U : List_Assert(msg, "linked_list.c", __LINE__))
static void List_Assert(char *msg, char *file, uint32_t line)
{
    printf("%s, assertion failed, file=%s, line=%lu\n", msg, file, line);
}
#else
#define LIST_ASSERT(expr, msg) ((void)0U)
#endif

/**
 * @brief create new node
 * @param data data field of new node
 * @retval return handle of node. return NULL on failure
 */
List_Node_t *List_New_Node(void *data)
{
    LIST_ASSERT(data, "NULL Passed");

    List_Node_t *new_node = (List_Node_t *)malloc(sizeof(List_Node_t));

    LIST_ASSERT(new_node, "malloc failed");

    if (new_node != NULL)
    {
        new_node->Data = data;
        new_node->Next = NULL;
    }

    return new_node;
}

/**
 * @brief add node to top of the list
 * @param head head of the list
 * @param new_node node to be added
 * @retval xreturn return 1 on success else 0
 */
uint8_t List_Add_At_Top(List_Node_t **head, List_Node_t *new_node)
{
    LIST_ASSERT(head, "NULL Passed");
    LIST_ASSERT(new_node, "NULL Passed");

    uint8_t xreturn = 0;

    if (head != NULL && new_node != NULL)
    {
        xreturn = 1;
        new_node->Next = *head;
        *head = new_node;
    }

    return xreturn;
}

/**
 * @brief add node after specified node
 * @param head head of the list
 * @param node node to be added
 * @param new_node node after witch new node is to be added
 * @retval xreturn return 1 on success else 0
 */
uint8_t List_Add_After(List_Node_t **head, List_Node_t *node, List_Node_t *new_node)
{
    LIST_ASSERT(head, "NULL Passed");
    LIST_ASSERT(node, "NULL Passed");
    LIST_ASSERT(new_node, "NULL Passed");

    uint8_t xreturn = 0;

    if (head != NULL && *head != NULL && node != NULL && new_node != NULL)
    {
        xreturn = 1;
        new_node->Next = node->Next;
        node->Next = new_node;
    }

    return xreturn;
}

/**
 * @brief add node before specified node
 * @param head head of the list
 * @param node node to be added
 * @param new_node node before witch new node is to be added
 * @retval xreturn return 1 on success else 0
 */
uint8_t List_Add_Before(List_Node_t **head, List_Node_t *node, List_Node_t *new_node)
{
    LIST_ASSERT(head, "NULL Passed");
    LIST_ASSERT(node, "NULL Passed");
    LIST_ASSERT(new_node, "NULL Passed");

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

/**
 * @brief add node at the end of the list
 * @param head head of the list
 * @param new_node node is to be added
 * @retval xreturn return 1 on success else 0
 */
uint8_t List_Add_At_End(List_Node_t **head, List_Node_t *new_node)
{
    LIST_ASSERT(head, "NULL Passed");
    LIST_ASSERT(new_node, "NULL Passed");

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

/**
 * @brief delete node from op of the list
 * @param head head of the list
 * @retval xreturn return 1 on success else 0
 */
uint8_t List_Delete_From_Top(List_Node_t **head)
{
    LIST_ASSERT(head, "NULL Passed");

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

/**
 * @brief delete node from bottom of the list
 * @param head head of the list
 * @retval xreturn return 1 on success else 0
 */
uint8_t List_Delete_From_End(List_Node_t **head)
{
    LIST_ASSERT(head, "NULL Passed");

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

/**
 * @brief delete specified node
 * @param head head of the list
 * @param node node to be deleted
 * @retval xreturn return 1 on success else 0
 */
uint8_t List_Delete_Node(List_Node_t **head, List_Node_t *node)
{
    LIST_ASSERT(head, "NULL Passed");
    LIST_ASSERT(node, "NULL Passed");

    uint8_t xreturn = 0;

    if (head != NULL && *head != NULL && node != NULL)
    {
        /* check if it is the fist node */
        if (*head == node)
        {
            xreturn = 1;
            free(*head);
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

/**
 * @brief delete all nodes in the list
 * @param head head of the list
 * @retval xreturn return 1 on success else 0
 */
uint8_t List_Delete_All(List_Node_t **head)
{
    LIST_ASSERT(head, "NULL Passed");

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

/**
 * @brief find node with specified data field in the list
 * @param head head of the list
 * @param data data field to search for
 * @retval List_Node_t return handle of node if success else return NULL
 */
List_Node_t *List_Find_Node(List_Node_t **head, void *data)
{
    LIST_ASSERT(head, "NULL Passed");
    LIST_ASSERT(data, "NULL Passed");

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

/**
 * @brief list as stack, push data on top of the stack
 * @param head head of the list acting as stack
 * @param data data to be push onto stack
 * @retval return 1 if success else 0
 */
uint8_t List_Push(List_Node_t **head, void *data)
{
    LIST_ASSERT(head, "NULL Passed");
    LIST_ASSERT(data, "NULL Passed");

    return List_Add_At_Top(head, List_New_Node(data));
}

/**
 * @brief list as stack, pop data from top of the stack
 * @param head head of the list acting as stack
 * @param data where data will be retrieved
 * @retval return 1 if success else 0
 */
uint8_t List_Pop(List_Node_t **head, void **data)
{
    LIST_ASSERT(head, "NULL Passed");
    LIST_ASSERT(data, "NULL Passed");

    if (head != NULL && *head != NULL)
    {
        *data = (*head)->Data;
    }

    return List_Delete_From_Top(head);
}

/**
 * @brief list as queue, push data into the queue
 * @param head head of the list acting as queue
 * @param data data to be queued
 * @retval return 1 if success else 0
 */
uint8_t List_Enqueue(List_Node_t **head, void *data)
{
    LIST_ASSERT(head, "NULL Passed");
    LIST_ASSERT(data, "NULL Passed");

    return List_Add_At_End(head, List_New_Node(data));
}

/**
 * @brief list as queue, enqueue data from queue
 * @param head head of the list acting as queue
 * @param data where data will be retrieved
 * @retval return 1 if success else 0
 */
uint8_t List_Dequeue(List_Node_t **head, void **data)
{
    LIST_ASSERT(head, "NULL Passed");
    LIST_ASSERT(data, "NULL Passed");

    if (head != NULL && *head != NULL)
    {
        *data = (*head)->Data;
    }

    return List_Delete_From_Top(head);
}

/**
 * @brief returns number on nodes in list
 * @param head head of the list
 * @retval count number of nodes
 */
uint16_t List_Get_Count(List_Node_t **head)
{
    LIST_ASSERT(head, "NULL Passed");

    uint16_t count = 0;

    for (List_Node_t *temp = *head; temp != NULL; temp = temp->Next)
    {
        count++;
    }

    return count;
}
