#ifndef DLL_OP_H
#define DLL_OP_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
    Node* tail;
    int size;
} List;

Node* getNode(int key) {
    Node* newnode = (Node*)malloc(sizeof(Node));
    newnode->key = key;
    newnode->prev = NULL;
    newnode->next = NULL;
    return newnode;
}

void initList(List* list) {
    list->head = list->tail = NULL;
    list->size = 0;
}

void innodeFront(List* list, int key) {
    Node* newnode = getNode(key);
    if (list->head == NULL) {
        list->head = list->tail = newnode;
    }
    else {
        newnode->next = list->head;
        list->head->prev = newnode;
        list->head = newnode;
    }
    
    list->size++;
}

void push_front_node(List* list, Node* node) {
    if (list->head == NULL) {
        list->head = list->tail = node;
    }
    else {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }

    list->size++;
}

Node* pop_tail(List* list) {
    if (list->tail == NULL)
        return NULL;

    Node* tail = list->tail;
    if (tail->prev) {
        tail->prev->next = NULL;
        list->tail = tail->prev;
    } else {
        list->head = list->tail = NULL;
    }

    list->size--;
    return tail;
}

void removeNode(List* list, Node* node) {
    if (node == NULL)
        return;

    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;

    free(node);
    list->size--;
}

Node* detachNode(List* list, Node* node) {
    if (node == NULL)
        return NULL;

    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;

    node->prev = node->next = NULL;
    list->size--;

    return node;
}

void displayList(List* list, const char* label) {
    printf("%s: [ ", label);
    Node* curr = list->head;

    while (curr) {
        printf("%d ", curr->key);
        curr = curr->next;
    }
    printf("]\n");
}

#endif
