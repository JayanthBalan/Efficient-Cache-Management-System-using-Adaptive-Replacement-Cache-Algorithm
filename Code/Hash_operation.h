#ifndef Hash_op_H
#define Hash_op_H

#include "DLL_op.h"
#include <math.h>

#define EMPTY 0
#define OCCUPIED 1
#define DELETED -1

#define A 0.6180339887

typedef struct HashEntry {
    int key;
    Node* node;
    int state;
} HashEntry;

typedef struct HashTable {
    int size;
    HashEntry* table;
} HashTable;

void initHashTable(HashTable* ht, int size) {
    ht->size = size;
    ht->table = (HashEntry*) malloc(sizeof(HashEntry) * size);

    for (int i = 0; i < size; i++) {
        ht->table[i].state = EMPTY;
        ht->table[i].key = 0;
        ht->table[i].node = NULL;
    }
}

int hashFunc(int key, int size) {
    if (key < 0) key = -key;

    double frac = fmod(key * A, 1.0);
    return (int)(size * frac);
}

void hashInsert(HashTable* ht, int key, Node* node) {
    int original_idx = hashFunc(key, ht->size);
    int idx;
    for (int i = 0; i < ht->size; i++) {
        idx = (original_idx + i * i) % ht->size;
        if (ht->table[idx].state != OCCUPIED || ht->table[idx].key == key) {
            ht->table[idx].key = key;
            ht->table[idx].node = node;
            ht->table[idx].state = OCCUPIED;
            return;
        }
    }
    printf("Hash table full (insert failed)!\n");
}

Node* hashSearch(HashTable* ht, int key) {
    int original_idx = hashFunc(key, ht->size);
    int idx;
    for (int i = 0; i < ht->size; i++) {
        idx = (original_idx + i * i) % ht->size;
        if (ht->table[idx].state == EMPTY) break; // Not found
        if (ht->table[idx].state == OCCUPIED && ht->table[idx].key == key) {
            return ht->table[idx].node;
        }
    }
    return NULL;
}

void hashDelete(HashTable* ht, int key) {
    int original_idx = hashFunc(key, ht->size);
    int idx;
    for (int i = 0; i < ht->size; i++) {
        idx = (original_idx + i * i) % ht->size;
        if (ht->table[idx].state == EMPTY) break; 
        if (ht->table[idx].state == OCCUPIED && ht->table[idx].key == key) {
            ht->table[idx].state = DELETED;
            ht->table[idx].node = NULL;
            return;
        }
    }
}

#endif
