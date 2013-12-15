#include "dlinked_list.h"
#include <stdlib.h>
#include <assert.h>

int init_dll(TTR_List **list)
{
    *list = malloc(sizeof(TTR_List));
    if (*list == NULL)
        return 1;
    return 0;
}

static TTR_List_Node *create_dll_node(void *contents)
{
    TTR_List_Node *node;
    node = malloc(sizeof(TTR_List_Node));
    if (node == NULL) /* error allocating memory */
        return node;
    node->contents = contents;
    return node;
}

int dll_push_head(TTR_List *list, void *contents)
{
    TTR_List_Node *node;

    assert(list != NULL);

    node = create_dll_node(contents);
    if (node == NULL)
        return 1;
    node->prev = NULL;
    node->next = list->head;
    if (list->head != NULL)
        list->head->prev = node;
    else
        list->tail = node;
    list->head = node;
    return 0;
}

int dll_push_tail(TTR_List *list, void *contents)
{
    TTR_List_Node *node;

    assert(list != NULL);
    
    node = create_dll_node(contents);
    if (node == NULL)
        return 1;
    node->next = NULL;
    node->prev = list->tail;
    if (list->tail != NULL)
        list->tail->next = node;
    else
        list->head = node;
    list->tail = node;
    return 0;
}

void *dll_pop_head(TTR_List *list)
{
    TTR_List_Node *node;
    void *value;

    assert(list != NULL);

    node = list->head;
    if (node == NULL)
        return NULL;

    list->head = node->next;
    if (list->head != NULL)
        list->head->prev = NULL;
    else
        list->tail = NULL;

    value = node->contents;
    free(node);
    return value;
}

void *dll_pop_tail(TTR_List *list)
{
    TTR_List_Node *node;
    void *value;

    assert(list != NULL);

    node = list->tail;
    if (node == NULL)
        return NULL;

    list->tail = node->prev;
    if (list->tail != NULL)
        list->tail->next = NULL;
    else
        list->head = NULL;

    value = node->contents;
    free (node);
    return value;
}

int dll_contains(TTR_List *list, void *contents)
{
    TTR_List_Node *node;

    assert(list != NULL);
    
    for (node = list->head; node != NULL; node = node->next) {
        if (node->contents == contents)
            return 1;
    }
    return 0;
}
