#include "hash.h"
#include "table_stack.h"
#include <string.h>

HashTable* createHashTable() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    ht->size = MAX_TABLE_SIZE;
    ht->table = (Symbol**)malloc(sizeof(Symbol*) * MAX_TABLE_SIZE);
    for (int i = 0; i < MAX_TABLE_SIZE; ++i) {
        ht->table[i] = NULL;
    }
    return ht;
}

void destroyHashTable(HashTable* ht) {
    for (int i = 0; i < ht->size; ++i) {
        Symbol* current = ht->table[i];
        while (current != NULL) {
            Symbol* temp = current;
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

item_data* insertSymbol(HashTable* ht, char* name, bool *internal_error) {
    if (findSymbol(ht, name)) {
        *internal_error = false;
        return NULL;
    }

    unsigned int index = hash(name, ht->size);

    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));

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
//    if (!newSymbol->data) printf("inside\n");

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
    newSymbol->data.type = 'u';
    newSymbol->data.defined = false;
    newSymbol->data.global = false;
    newSymbol->next = NULL;

    newSymbol->next = ht->table[index];
    ht->table[index] = newSymbol;

    return &newSymbol->data;
}

Symbol* findSymbol(HashTable* ht, char* name) {
    unsigned int index = hash(name, ht->size);
    Symbol* current = ht->table[index];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

Symbol *findSymbol_global(t_table_stack *s, char *name)
{
    for (t_table_stack_elem *tmp = s->top; tmp != NULL; tmp = tmp->next)
    {
        Symbol *symbol = findSymbol(tmp->table, name);
        if (symbol->data.global && symbol != NULL)
            return symbol;
    }
    return NULL;
}

void removeSymbol(HashTable* ht, char* name) {
    unsigned int index = hash(name, ht->size);
    Symbol* current = ht->table[index];
    Symbol* prev = NULL;
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