#include <string.h>
#include "../structures/string.h"
#include "stdio.h"

#ifndef HASH_H
#define HASH_H

#define MAX_TABLE_SIZE 101

typedef enum {
    IT_DOUBLE,
    IT_INT,
    IT_STRING,
    IT_ANY,
    IT_NIL,
    IT_UNDEF
} item_type;


typedef struct
{
    string_ptr params;      // i - int, s - string, d - double, a - any; if BIG, then may be NULL
    char **id_names; // length = params.mem_allocated
    char* id;
    item_type type;
    bool nil_possibility;
    bool defined;
    bool global;
} item_data;

typedef struct Symbol {
    char *name;
    item_data data;
    struct Symbol* next;
} Symbol;

typedef struct HashTable {
    int size;
    Symbol** table;
} HashTable;

HashTable* createHashTable();

void destroyHashTable(HashTable* ht);

unsigned int hash(char* str, int size);

item_data* insertSymbol(HashTable* ht, char* name, bool *internal_error);

Symbol* findSymbol(HashTable* ht, char* name);

void removeSymbol(HashTable* ht, char* name);

#endif
