#ifndef H_HASH_TABLE
#define H_HASH_TABLE

#define NUM_BUCKETS 101

typedef struct hash_node {
    struct hash_node *next;
    char *key;
    void *value;
} Hash_Node;

typedef struct hash_table {
    Hash_Node *table[NUM_BUCKETS];
} Hash_Table;

/*
 * Allocate space and point a Hash_Table at it.
 * Returns 0 on success, non-0 otherwise.
 */
int init_hash_table(Hash_Table **table);

/*
 * Free the space allocated to a Hash_Table.
 */
void free_hash_table(Hash_Table *table);

/*
 * Lookup and return the value associated with a key.
 */
void *hash_table_lookup(Hash_Table *table, char *key);

/*
 * Add a value to the hash table.
 */
int hash_table_add(Hash_Table *table, char *key, void *value);

#endif /* H_HASH_TABLE */
