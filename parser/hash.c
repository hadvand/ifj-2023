#include <stdlib.h>
#include <string.h>
#include "hash.h"

HashTable* createHashTable() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    ht->size = TABLE_SIZE;
    ht->table = (Symbol**)malloc(sizeof(Symbol*) * TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; ++i) {
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

Symbol* insertSymbol(HashTable* ht, char* name) {

    unsigned int index = hash(name, ht->size);

    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));

    if (newSymbol == NULL) {
        return NULL;
    }

    newSymbol->name = (char*)malloc(strlen(name) + 1);
    if (newSymbol->name == NULL) {

        free(newSymbol);
        return NULL;
    }

    strcpy(newSymbol->name, name);

    newSymbol->next = ht->table[index];
    ht->table[index] = newSymbol;

    return newSymbol;
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