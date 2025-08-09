#ifndef LINKED_LIST
#define LINKED_LIST

typedef struct linked_list
{
    struct node *head;
    struct node *tail;
    int size;
    int max_size;

} linked_list;

typedef struct node
{
    unsigned long value;
    struct node *next;

} node;


linked_list *createLinkedList(int max_size);
node *createNode();
int enqueue(linked_list *ll, node *nde);
int dequeue(linked_list *ll);
int setNext(node *nde, node *old);
int freeNode(node *nde);
int freeLinkedList(linked_list *ll);
int freeAll(linked_list *ll);
double getAverage(linked_list *ll);
node *createNodeSet(unsigned long val);
int setValue(node *nde, unsigned long val);
#endif