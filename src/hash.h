/**
 * @file hash.h
 * @author Oleg Borshch (xborsh00)
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
    bool global;
    bool is_function;
} item_data;

/**
 * @brief hashtable item representation
 */
typedef struct symbol {
    char *name;
    item_data data;
    struct symbol* next;
} symbol;

/**
 * @brief hashtable representation
 */
typedef struct HashTable {
    int size;
    symbol** table;
} hash_table;

/**
 * @brief function for creating hashtable
 */
hash_table* create_hash_table();

/**
 * @brief hashtable destructor
 */
void destroy_hash_table(hash_table* ht);

/**
 * @brief function for generating hash
 */
unsigned int hash(char* str, int size);

/**
 * @brief function for inserting a new symbol into the hashtable
 */
item_data* insert_symbol(hash_table* ht, char* name, bool *internal_error);

/**
 * @brief function for finding needed symbol inside the hashtable
 */
symbol* find_symbol(hash_table* ht, char* name);

/**
 * @brief function for removing a symbol from the hashtable
 */
void remove_symbol(hash_table* ht, char* name);

/**
 * @brief function for creating temporary variable
 */
item_data create_default_item(void);

#endif
