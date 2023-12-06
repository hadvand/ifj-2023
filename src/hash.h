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
    bool is_function;
    bool global;
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
 * @brief Function to create an empty hashtable
 * @return A pointer to the newly created hashtable
 */
hash_table* create_hash_table();

/**
 * @brief Destructor for the hashtable, releasing allocated memory
 * @param ht Pointer to the hashtable to be destroyed
 */
void destroy_hash_table(hash_table* ht);

/**
 * @brief Function to generate a hash value for a given string
 * @param str The input string to be hashed
 * @param size Size of the hashtable (used in the hashing algorithm)
 * @return The generated hash value
 */
unsigned int hash(char* str, int size);

/**
 * @brief Function to insert a new symbol into the hashtable
 * @param ht Pointer to the hashtable
 * @param name The name of the symbol (variable or function identifier)
 * @param internal_error Pointer to a boolean variable indicating internal errors during insertion
 * @return Pointer to the item_data structure containing information about the inserted symbol
 */
item_data* insert_symbol(hash_table* ht, char* name, bool *internal_error);

/**
 * @brief Function to find a symbol inside the hashtable
 * @param ht Pointer to the hashtable
 * @param name The name of the symbol to be found
 * @return Pointer to the symbol structure if found, NULL otherwise
 */
symbol* find_symbol(hash_table* ht, char* name);

/**
 * @brief Function to remove a symbol from the hashtable
 * @param ht Pointer to the hashtable
 * @param name The name of the symbol to be removed
 */
void remove_symbol(hash_table* ht, char* name);

/**
 * @brief Function to create a temporary variable with default values
 * @return An item_data structure representing the default item
 */
item_data create_default_item(void);

#endif
