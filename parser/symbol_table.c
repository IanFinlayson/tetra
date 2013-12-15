#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tetra_hdr.h"

int init_symbol_table(Symbol_Table **table)
{
    *table = malloc(sizeof(Symbol_Table));
    if (table == NULL)
        return 1;
    (*table)->sp = 0;
    (*table)->next_scope = 1;
    return init_hash_table(&(*table)->table);
}

static int init_symbol_entry_list(Symbol_Entry_List **list, char *name)
{
    assert(name != NULL);
    assert(list != NULL);

    *list = malloc(sizeof(Symbol_Entry_List));
    if (list == NULL)
        return 1;
    (*list)->name = strdup(name);
    if ((*list)->name == NULL)
        return 1;
    return 0;
}

TTR_Node *symbol_table_lookup(Symbol_Table *table, char *name)
{
    Symbol_Entry *pervasive = NULL, *best = NULL, *entry;
    Symbol_Entry_List *chain;
    int scope;
    int scope_index = table->sp-1;

    assert(table != NULL);
    assert(name != NULL);

    chain = hash_table_lookup(table->table, name);
    if (chain == NULL)
        return NULL;
    for (entry = chain->first; entry != NULL; entry = entry->next) {
        if (entry->scope == 0) {
            pervasive = entry;
        } else {
            for (   scope = table->scope_stack[scope_index];
                    scope_index >= 0;
                    scope_index--) {
                if (scope == entry->scope) {
                    best = entry;
                    break;             
                } else if (best != NULL && scope == best->scope) {
                    break;
                } /* If using closed scopes, put test and break here */
            }
        }
    }
    if (best != NULL) {
    /* If using import/export entries, change best to real entry here */
        return best->node;
    } else if (pervasive != NULL) {
        return pervasive->node;
    } else {
        return NULL;
    }
}

int symbol_table_add(Symbol_Table *table, char *name, TTR_Node *node)
{
    Symbol_Entry_List *chain;
    Symbol_Entry *entry;

    assert(table != NULL);
    assert(name != NULL);
    assert(node != NULL);

    chain = hash_table_lookup(table->table, name);
    if (chain == NULL) {
        if (init_symbol_entry_list(&chain, name))
            return 1;
        hash_table_add(table->table, name, chain);
    }
    entry = malloc(sizeof(Symbol_Entry));
    if (entry == NULL)
        return 1; 
    entry->name = strdup(name);
    if (entry->name == NULL)
        return 1;
    entry->node = node;
    entry->scope = table->scope_stack[table->sp-1];
    entry->next = chain->first;
    chain->first = entry;
    return 0;
}

void symbol_table_enter_next_scope(Symbol_Table *table)
{
    assert(table != NULL);

    symbol_table_enter_scope(table, table->next_scope);
    table->next_scope++;
}

void symbol_table_enter_scope(Symbol_Table *table, int scope)
{
    assert(table != NULL);
    assert(table->sp != MAX_SCOPES);

    table->scope_stack[table->sp++] = scope;
}

void symbol_table_leave_scope(Symbol_Table *table)
{
    assert(table != NULL);
    assert(table->sp != 0);

    table->sp--;
}

void TTR_set_ident_data_type(
        Symbol_Table *table, TTR_Node *identifier, int type)
{
    TTR_Node *sym_node;

    assert(table != NULL);
    assert(identifier != NULL);
    assert(N_TYPE(identifier) == N_IDENTIFIER ||
            N_TYPE(identifier) == N_FUNCDEF);

    sym_node = symbol_table_lookup(table, N_STR(identifier));
    if (sym_node == NULL) {
        N_DTYPE(identifier) = type;
        symbol_table_add(table, N_STR(identifier), identifier);
    } else {
        if (N_DTYPE(sym_node) == UNDEFINED_T) {
            N_DTYPE(identifier) = type;
            N_DTYPE(sym_node) = type;
        } else {
            N_DTYPE(identifier) = N_DTYPE(sym_node);
        }
    }
}
