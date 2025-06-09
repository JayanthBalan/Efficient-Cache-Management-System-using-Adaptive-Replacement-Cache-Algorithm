#include "Hash_op.h"

#define INPUT_FILE "D:\\Coding and Simulations\\Visual Studio Code (C)\\Project Nexus\\in_cache_dat.txt"
#define DECAY_INTERVAL 3

int capacity;
int p;
int requestCount = 0;

List T1, T2, B1, B2;
HashTable T1HT, T2HT, B1HT, B2HT;

void initHashTables(int);
void initARC(int);
void replace(int);
int arcAccess(int);
void printLists();
void cleanupStaleEntries();

int main() {
    FILE *fp = fopen(INPUT_FILE, "r");
    if (!fp) {
        return 1;
    }

    char line[100];
    int cap, key;

    if (fgets(line, sizeof(line), fp)) {
        sscanf(line, "Cache Capacity: %d", &cap);
    }
    initARC(cap);

    int hits = 0, misses = 0, accessCount = 0;

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "Request Key: %d", &key);
        if (key == -1)
            break;

        accessCount++;

        int res = arcAccess(key);
        if (res == 1) {
            printf("Cache HIT for key %d\n", key);
            hits++;
        }
        else {
            printf("Cache MISS for key %d\n", key);
            misses++;
        }

        printLists();

        if (accessCount % 50 == 0) {
            double hitRate = ((double)hits / accessCount) * 100.0;
            printf("STATS AFTER %d ACCESSES\n", accessCount);
            printf("Hits: %d | Misses: %d | Hit Rate: %.2f%%\n", hits, misses, hitRate);
            printf("\n");
        }
    }

    printf("\n:::Final Statistics:::\n");
    printf("Total Accesses: %d\n", accessCount);
    printf("Total Hits    : %d\n", hits);
    printf("Total Misses  : %d\n", misses);
    printf("Final Hit Rate: %.2f%%\n", ((double)hits / accessCount) * 100.0);
    printf("\n");

    fclose(fp);
    return 0;
}


void initHashTables(int cap) {
    int ht_size = cap * 2 + 1;
    initHashTable(&T1HT, ht_size);
    initHashTable(&T2HT, ht_size);
    initHashTable(&B1HT, ht_size);
    initHashTable(&B2HT, ht_size);
}

void initARC(int cap) {
    capacity = cap;
    p = 0;
    initList(&T1);
    initList(&T2);
    initList(&B1);
    initList(&B2);
    initHashTables(cap);
}

void cleanupStaleEntries() {
    int numToClean = 2;

    while (B1.size > 0 && numToClean--) {
        Node* stale = pop_tail(&B1);
        if (stale) {
            hashDelete(&B1HT, stale->key);
            free(stale);
        }
    }

    numToClean = 2;
    while (B2.size > 0 && numToClean--) {
        Node* stale = pop_tail(&B2);
        if (stale) {
            hashDelete(&B2HT, stale->key);
            free(stale);
        }
    }
}

void replace(int key) {
    if (T1.size > 0 && (T1.size > p || (hashSearch(&B2HT, key) != NULL && T1.size == p))) {
        Node* old = pop_tail(&T1);
        hashDelete(&T1HT, old->key);
        innodeFront(&B1, old->key);
        Node* ghost = B1.head;
        hashInsert(&B1HT, ghost->key, ghost);
        free(old);
    } else {
        Node* old = pop_tail(&T2);
        hashDelete(&T2HT, old->key);
        innodeFront(&B2, old->key);
        Node* ghost = B2.head;
        hashInsert(&B2HT, ghost->key, ghost);
        free(old);
    }
}

int arcAccess(int key) {
    Node* node = hashSearch(&T1HT, key);
    if (node != NULL) {
        detachNode(&T1, node);
        hashDelete(&T1HT, key);
        push_front_node(&T2, node);
        hashInsert(&T2HT, key, node);
        return 1;
    }

    node = hashSearch(&T2HT, key);
    if (node != NULL) {
        detachNode(&T2, node);
        hashDelete(&T2HT, key);
        push_front_node(&T2, node);
        hashInsert(&T2HT, key, node);
        return 1;
    }

    if (hashSearch(&B1HT, key) != NULL) {
        int delta = (B2.size >= B1.size) ? 1 : (B1.size / (B2.size ? B2.size : 1));
        p = (p + delta > capacity) ? capacity : p + delta;
        replace(key);
        hashDelete(&B1HT, key);
        innodeFront(&T2, key);
        Node* newNode = T2.head;
        hashInsert(&T2HT, key, newNode);
        return 0;
    }

    if (hashSearch(&B2HT, key) != NULL) {
        int delta = (B1.size >= B2.size) ? 1 : (B2.size / (B1.size ? B1.size : 1));
        p = (p - delta < 0) ? 0 : p - delta;
        replace(key);
        hashDelete(&B2HT, key);
        innodeFront(&T2, key);
        Node* newNode = T2.head;
        hashInsert(&T2HT, key, newNode);
        return 0;
    }

    if (T1.size + T2.size == capacity) {
        if (T1.size < capacity) {
            if (B2.size > 0) {
                Node* tailGhost = pop_tail(&B2);
                hashDelete(&B2HT, tailGhost->key);
                free(tailGhost);
            }
            replace(key);
        } else {
            Node* old = pop_tail(&T1);
            hashDelete(&T1HT, old->key);
            innodeFront(&B1, old->key);
            Node* ghost = B1.head;
            hashInsert(&B1HT, ghost->key, ghost);
            free(old);
        }
    } else if (T1.size + T2.size < capacity && (T1.size + T2.size + B1.size + B2.size) >= capacity) {
        if ((T1.size + T2.size + B1.size + B2.size) >= 2 * capacity) {
            if (B2.size > 0) {
                Node* tailGhost = pop_tail(&B2);
                hashDelete(&B2HT, tailGhost->key);
                free(tailGhost);
            }
        }
        replace(key);
    }

    innodeFront(&T1, key);
    Node* newNode = T1.head;
    hashInsert(&T1HT, key, newNode);
    return 0;
}

void printLists() {
    displayList(&T1, "T1");
    displayList(&T2, "T2");
    displayList(&B1, "B1");
    displayList(&B2, "B2");
    printf("---------------\n");
}
