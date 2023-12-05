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

/**
 * @brief enum for readable code errors
 */
typedef enum {
    IT_DOUBLE,
    IT_INT,
    IT_STRING,
    IT_ANY,
    IT_NIL,
    IT_UNDEF,
    IT_BOOL
} item_type;

/**
 * @brief enum for readable code errors
 */
typedef struct
{
    string_ptr params;      // i - int, s - string, d - double, a - any; if BIG, then may be NULL
    char **id_names; // length = params.last_index
    char* id;
    item_type type; // return type/ var type
    bool nil_possibility;
    bool is_let;
    //bool it_is_nil;
    bool defined;
    bool is_function;
} item_data;

/**
 * @brief
 */
typedef struct symbol {
    char *name;
    item_data data;
    struct symbol* next;
} symbol;

/**
 * @brief
 */
typedef struct HashTable {
    int size;
    symbol** table;
} hash_table;

/**
 * @brief
 */
hash_table* create_hash_table();

/**
 * @brief
 */
void destroy_hash_table(hash_table* ht);

/**
 * @brief
 */
unsigned int hash(char* str, int size);

/**
 * @brief
 */
item_data* insert_symbol(hash_table* ht, char* name, bool *internal_error);

/**
 * @brief
 */
symbol* find_symbol(hash_table* ht, char* name);

/**
 * @brief
 */
void remove_symbol(hash_table* ht, char* name);

/**
 * @brief
 */
item_data create_default_item(void);

#endif
