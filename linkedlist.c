/* Created by Matthew Sundquist*/

#include <stdlib.h>
#include "linkedlist.h"

linked_list *createLinkedList(int max_size)
{
    linked_list *list;

    if ((list = (linked_list *)malloc(sizeof(linked_list))) == NULL)
        return NULL;

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->max_size = max_size;

    return list;
}

node *createNode()
{
    node *nde;

    if ((nde = (node *)malloc(sizeof(node))) == NULL)
        return NULL;
    
    nde->next = NULL;
    nde->value = 0;
    
    return nde;
}



int enqueue(linked_list *ll, node *nde)
{
    if (ll == NULL || nde == NULL)
        return -1;

    if (ll->size == ll->max_size)
    {
        dequeue(ll);
    }


    if (ll->head == NULL && ll->tail == NULL)
    {
        ll->head = nde;
        ll->tail = nde;
    }

    else
    {
        if (setNext(nde, ll->head) < 0)
            return -1;
        ll->head = nde;
    }

    ll->size++;

    return 0;
}

int dequeue(linked_list *ll)
{
    if (ll == NULL)
        return -1;
    
    if (ll->size < 1)
        return -1;



    if (ll->tail == ll->head)
    {


        free(ll->head);

        ll->tail = NULL;
        ll->head = NULL;
        
    }

    else
    {

        node *walker = ll->head;

        while (walker->next != ll->tail)
        {
            walker = walker->next;
        }

        if (setNext(walker, NULL) < 0)
            return -1;

        if (freeNode(ll->tail) < 0)
            return -1;

        ll->tail = walker;

    }

    ll->size--;

    return 0;
}

/* helper */
int setNext(node *nde, node *old)
{
    if (nde == NULL)
        return -1;

    nde->next = old;

    return 0;
}

/* helper */
int freeNode(node *nde)
{
    if (nde == NULL)
        return -1;

    free(nde);

    return 0;
}

int freeLinkedList(linked_list *ll)
{
    if (ll == NULL)
        return -1;
    
    free(ll);

    return 0;
}

int freeAll(linked_list *ll)
{
    if (ll == NULL)
        return -1;
    
    node *cur = ll->head;
    node *prev = NULL;

    while (cur != NULL)
    {
        prev = cur;

        cur = cur->next;

        free(prev);
    }

    if (freeLinkedList(ll) < 0)
        return -1;

    return 0;
}

double getAverage(linked_list *ll)
{
    if (ll == NULL)
        return -1;
    
    if (ll->head == ll->tail)
        return ll->head->value;

    node *cur = ll->head;
    unsigned long sum = 0;
    int count = 0;

    while (cur != NULL)
    {
        sum += cur->value;
        count++;
        cur = cur->next;
    }

    return (double) sum/count;
}

int setValue(node *nde, unsigned long val)
{
    if (nde == NULL)
        return -1;

    nde->value = val;
    
    return 0;
}

node *createNodeSet(unsigned long val)
{
    node *nde = createNode();

    setValue(nde, val);

    return nde;
}
