/*
     Implementation of the stack data type (fixed size) with operations:
     - Add items;
     - Getting an element from the top of the stack.

     Implementation of the deque (dynamic) data type with operations:
     - Adding an element to the beginning and end;
     - Delete an element from the beginning and end.
*/
#include <stdio.h>
#include <stdlib.h>

#define STACK_MAX_SIZE 20
#define STACK_OVERFLOW  1
#define STACK_UNDERFLOW 2
#define DEQUE_OVERFLOW 3

typedef struct stack_struct stack_type;
typedef int stack_val_type;

typedef struct deque_struct deque_type;
typedef int deque_val_type;

struct stack_struct {
    stack_val_type data[STACK_MAX_SIZE];
    size_t size;
};

struct node_struct {
    struct node_struct *next;
    struct node_struct *prev;
    deque_val_type val;
};

struct deque_struct {
    struct node_struct *head;
    struct node_struct *tail;
};

/* Adding an item to the stack: */
void stack_push(stack_type *st, stack_val_type value)
{
    if(st->size >= STACK_MAX_SIZE)
    {
        exit(STACK_OVERFLOW);
    }
    st->data[st->size] = value;
    st->size++;
}

/* Getting an item from the top of the stack and moving to the next one: */
stack_val_type stack_pop(stack_type *st)
{
    if(st->size <= 0)
    {
        exit(STACK_UNDERFLOW);
    }
    st->size--;
    return st->data[st->size];
}

/* Getting an element from the top of the stack: */
stack_val_type stack_peek(stack_type *st)
{
    if(st->size <= 0)
    {
        exit(STACK_UNDERFLOW);
    }
    return st->data[st->size - 1];
}

void stack_print(stack_type *st)
{
    int i;
    unsigned long len = st->size - 1;
    
    if(!(st->size))
    {
        printf("stack is empty\n");
        return;
    }
    
    printf("stack consists of %zu elements: ", st->size);
    
    for(i = 0; i < len; i++)
    {
        printf("%d | ", st->data[i]);
    }
    
    if(st->size != 0)
    {
        printf("%d", st->data[i]);
    }
    printf("\n");
}

/* Adding an element at the beginning of the deque: */
void deque_push_front(deque_type *d, deque_val_type v)
{
    struct node_struct *n = malloc(sizeof(struct node_struct));
    
    if(n == NULL)
    {
        exit(DEQUE_OVERFLOW);
    }
    n->val = v;
    n->next = d->head;
    n->prev = NULL;
    
    if(d->tail == NULL)
    {
        d->head = d->tail = n;
    }
    else
    {
        d->head->prev = n;
        d->head = n;
    }
}

/* Adding an element to the end of the deque: */
void deque_push_back(deque_type *d, deque_val_type v)
{
    struct node_struct *n = malloc(sizeof(struct node_struct));
    
    if(n == NULL)
    {
        exit(DEQUE_OVERFLOW);
    }
    n->val = v;
    n->prev = d->tail;
    n->next = NULL;
    
    if(d->head == NULL)
    {
        d->head = d->tail = n;
    }
    else
    {
        d->tail->next = n;
        d->tail = n;
    }
}

/* Deleting an element from the beginning of the deque: */
deque_val_type deque_pop_front(deque_type *d)
{
    deque_val_type v = d->head->val;
    struct node_struct *n = d->head;
    
    if(d->head == d->tail)
    {
        d->head = d->tail = NULL;
    }
    else
    {
        d->head = n->next;
    }
    free(n);
    return v;
}

/* Deleting an element from the end of the deque: */
deque_val_type deque_pop_back(deque_type *d)
{
    deque_val_type v = d->tail->val;
    struct node_struct *n = d->tail;
    
    if(d->head == d->tail)
    {
        d->head = d->tail = NULL;
    }
    else
    {
        d->tail = n->prev;
    }
    free(n);
    return v;
}

void deque_print(deque_type *d)
{
    if((d->head == d->tail) && (d->head == NULL))
    {
        printf("deque is empty\n");
        return;
    }
    struct node_struct *n = d->head;
    printf("deque consists of: ");
    
    while(n != d->tail)
    {
        printf("%d | ", n->val);
        n = n->next;
    }
    printf("%d\n", n->val);
}

int main()
{
    /* Example of working with a stack: */
    stack_type stack;
    stack.size = 0;
     
    stack_push(&stack, 3);
    stack_print(&stack);
    stack_push(&stack, 5);
    stack_print(&stack);
    stack_push(&stack, 7);
    
    stack_print(&stack);
    printf("stack_pop element: %d\n", stack_pop(&stack));
    stack_print(&stack);
    printf("stack_pop element: %d\n", stack_pop(&stack));
    stack_print(&stack);
    printf("stack_pop element: %d\n", stack_pop(&stack));
    stack_print(&stack);
    
    /* Example of working with a deque: */
    deque_type deque;
    printf("\n");
    
    deque_push_front(&deque, 2);
    deque_print(&deque);
    deque_push_back(&deque, 4);
    deque_print(&deque);
    deque_push_front(&deque, 6);
    
    deque_print(&deque);
    printf("deque_pop_back element: %d\n", deque_pop_back(&deque));
    deque_print(&deque);
    printf("deque_pop_front element: %d\n", deque_pop_front(&deque));
    deque_print(&deque);
    printf("deque_pop_back element: %d\n", deque_pop_back(&deque));
    deque_print(&deque);
    
    return 0;
}
