#ifndef H_DLINKED_LIST
#define H_DLINKED_LIST

struct dll_node {
    struct dll_node *next, *prev;
    void *contents;
};

struct dll {
    struct dll_node *head, *tail;
};

typedef struct dll_node TTR_List_Node;
typedef struct dll TTR_List;

/*
 * Initialize a dll.
 * Return 0 if all goes well, non-0 otherwise.
 */
int init_dll(TTR_List **list);

/*
 * Push contents onto the beginning of the stack.
 * Return 0 if all goes well, non-0 otherwise.
 */
int dll_push_head(TTR_List *list, void *contents);

/*
 * Push contents onto the end of the stack.
 * Return 0 if all goes well, non-0 otherwise.
 */
int dll_push_tail(TTR_List *list, void *contents);

/*
 * Pop and return the contents from the head of the list.
 */
void *dll_pop_head(TTR_List *list);

/*
 * Pop and return the contents from the back of the list.
 */
void *dll_pop_tail(TTR_List *list);

/*
 * See if the contents are already contained in the list.
 * Equality testing is done by reference.
 *
 * Return 1 if the list already contains the pointer, 0 otherwise.
 */
int dll_contains(TTR_List *list, void *contents);

#endif /* H_DLINKED_LIST */
