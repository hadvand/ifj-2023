#ifndef HASH_H
#define HASH_H

#define TABLE_SIZE 101

typedef struct Symbol {
    char* name;
    struct Symbol* next;
} Symbol;

typedef struct HashTable {
    int size;
    Symbol** table;
} HashTable;

HashTable* createHashTable();

void destroyHashTable(HashTable* ht);

unsigned int hash(char* str, int size);

Symbol* insertSymbol(HashTable* ht, char* name);

Symbol* findSymbol(HashTable* ht, char* name);

void removeSymbol(HashTable* ht, char* name);

#endif
