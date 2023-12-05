/**
 * @file hash.h
 * @author Oleg Borsh (xborsh00)
 * @brief symtable for parser
 */

#include <string.h>
#include "string.h"
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
    char **id_names; // length = params.last_index
    char* id;
    item_type type; // return type/ var type
    bool nil_possibility;
    //bool it_is_nil;
    bool defined;
    bool global;
    bool is_function;
} item_data;

typedef struct symbol {
    char *name;
    item_data data;
    struct symbol* next;
} symbol;

typedef struct HashTable {
    int size;
    symbol** table;
} hash_table;

hash_table* create_hash_table();

void destroy_hash_table(hash_table* ht);

unsigned int hash(char* str, int size);

item_data* insert_symbol(hash_table* ht, char* name, bool *internal_error);

symbol* find_symbol(hash_table* ht, char* name);

void remove_symbol(hash_table* ht, char* name);

#endif
