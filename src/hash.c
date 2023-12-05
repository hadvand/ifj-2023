/**
 * @file hash.c
 * @author Oleg Borsh (xborsh00)
 * @brief symtable for parser
 */

#include "hash.h"
#include <string.h>

hash_table* create_hash_table() {
    hash_table* ht = (hash_table*)malloc(sizeof(hash_table));
    ht->size = MAX_TABLE_SIZE;
    ht->table = (symbol**)malloc(sizeof(symbol*) * MAX_TABLE_SIZE);
    for (int i = 0; i < MAX_TABLE_SIZE; ++i) {
        ht->table[i] = NULL;
    }
    return ht;
}

void destroy_hash_table(hash_table* ht) {
    for (int i = 0; i < ht->size; ++i) {
        symbol* current = ht->table[i];
        while (current != NULL) {
            symbol* temp = current;
            current = current->next;
            free(temp->name);
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
}

unsigned int hash(char* str, int size) {
    unsigned int hashValue = 0;
    while (*str != '\0') {
        hashValue = (hashValue * 31) + *str++;
    }
    return hashValue % size;
}

item_data* insert_symbol(hash_table* ht, char* name, bool *internal_error) {
    if (find_symbol(ht, name)) {
        *internal_error = false;
        return NULL;
    }

    unsigned int index = hash(name, ht->size);

    symbol* newSymbol = (symbol*)malloc(sizeof(symbol));

    if (newSymbol == NULL) {
        *internal_error = true;
        return NULL;
    }

    newSymbol->name = (char*)malloc(strlen(name) + 1);

    if (newSymbol->name == NULL) {

        *internal_error = true;
        free(newSymbol);
        return NULL;
    }

    if (!(newSymbol->name = (char *)malloc((strlen(name) + 1) * sizeof(char))))
    {
        free(newSymbol);
        *internal_error = true;
        return NULL;
    }

    if (!(newSymbol->data.params = (string_ptr)malloc(sizeof(string_ptr))))
    {
        free(newSymbol->name);
        free(newSymbol);
        *internal_error = true;
        return NULL;
    }
    if ((newSymbol->data.params = string_init()) == NULL)
    {
        free(newSymbol->name);
        free(newSymbol->data.params);
        free(newSymbol);
        *internal_error = true;
        return NULL;
    }


    strcpy(newSymbol->name, name);

    newSymbol->data.id = newSymbol->name;
    newSymbol->data.type = IT_UNDEF;
    newSymbol->data.is_function = false;
    newSymbol->next = NULL;

    newSymbol->next = ht->table[index];
    ht->table[index] = newSymbol;

    return &newSymbol->data;
}

symbol* find_symbol(hash_table* ht, char* name) {
    unsigned int index = hash(name, ht->size);
    symbol* current = ht->table[index];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void remove_symbol(hash_table* ht, char* name) {
    unsigned int index = hash(name, ht->size);
    symbol* current = ht->table[index];
    symbol* prev = NULL;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                ht->table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->name);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}