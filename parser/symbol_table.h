/*
 * Implements a LeBlanc Cook symbol table, as described in
 * Programming Language Pragmatics by Michael Scott, section 3.4.
 */
#ifndef H_SYMBOL_TABLE
#define H_SYMBOL_TABLE

#include <map>

#include "node.h" 

#define MAX_SCOPES 100

typedef struct sym_entry {
    char *name;
    TTR_Node *node;
    int scope;
    struct sym_entry *next;
} Symbol_Entry;

typedef struct sym_list {
    char *name;
    Symbol_Entry *first;
} Symbol_Entry_List;

typedef struct sym_table {
    Hash_Table *table;
    int scope_stack[MAX_SCOPES];
    int sp;
    int next_scope;
} Symbol_Table;

/*
 * Initialize the symbol table.
 * Returns 0 on success, non-0 otherwise.
 */
int init_symbol_table(Symbol_Table **table);

/*
 * Look up the best node for the current scope.
 * Returns NULL if no suitable node is found.
 */
TTR_Node *symbol_table_lookup(Symbol_Table *table, char *name);

/*
 * Add a symbol entry to the symbol table.
 * Returns 0 on success, non-0 otherwise.
 */
int symbol_table_add(Symbol_Table *table, char *name, TTR_Node *node);

/*
 * Enter the next free scope.
 */
void symbol_table_enter_next_scope(Symbol_Table *table);

/*
 * Enter a new scope.
 */
void symbol_table_enter_scope(Symbol_Table *table, int scope);

/*
 * Leave a scope.
 */
void symbol_table_leave_scope(Symbol_Table *table);

/*
 * Get the current scope.
 */
inline int symbol_table_current_scope(Symbol_Table *table);

/*
 * Set the type for an identifier.  If the identifier already has a
 * type in the current scope, do nothing.  Otherwise, check identifiers
 * in surrounding scopes until a type is found.  The global scope is only
 * checked in functions if the identifier was declared global before
 * the type checking occurs.
 */
void TTR_set_ident_data_type(
        Symbol_Table *table, TTR_Node *identifier, int type);

#endif /* H_SYMBOL_TABLE */
