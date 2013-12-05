#include "hash_table.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static unsigned knr_hash(char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % NUM_BUCKETS;
}

int init_hash_table(Hash_Table **table)
{
    *table = malloc(sizeof(Hash_Table));    
    if (*table == NULL)
        return 1;
    return 0;
}

void free_hash_table(Hash_Table *table)
{
    int i;

    assert(table != NULL);
    
    for (i = 0; i < NUM_BUCKETS; i++) {
        if (table->table[i] != NULL) {
            free(table->table[i]->key);
            free(table->table[i]);
        }
    }
    free(table);
}

void *hash_table_lookup(Hash_Table *table, char *key)
{
    Hash_Node *nl;

    assert(table != NULL);
    assert(key != NULL);

    for (nl = table->table[knr_hash(key)]; nl != NULL; nl = nl->next) {
        if (strcmp(key, nl->key) == 0) {
            return nl->value;
        }
    }
    return NULL;
}

int hash_table_add(Hash_Table *table, char *key, void *value)
{
    Hash_Node *node;
    unsigned hashval;

    assert(table != NULL);
    assert(key != NULL);

    if ((node = hash_table_lookup(table, key)) != NULL) {
        node->value = value;
        return 0;
    }
    node = malloc(sizeof(Hash_Node));
    if (node == NULL || (node->key = strdup(key)) == NULL) {
        return 1;
    }
    node->value = value;
    hashval = knr_hash(key);
    node->next = table->table[hashval];
    table->table[hashval] = node;
    return 0;
}
