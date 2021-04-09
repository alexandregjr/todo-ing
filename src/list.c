#include <stdlib.h>

#include "list.h"

void listT_push(listT *list, void *item) {
    nodeT *new_node = (nodeT *)calloc(1, sizeof(nodeT));
    new_node->value = item;

    if (!list->begin) {
        list->begin = new_node;
    } else {
        nodeT *curr = list->begin;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = new_node;
    }
    list->size++;
}

void listT_print(listT *list, void (*print)(void *, int)) {
    if (!list->begin)
        return;

    nodeT *curr = list->begin;
    int i = 0;
    while (curr) {
        if (print) {
            print(curr->value, i);
        } else {
            // no print provided
        }
        curr = curr->next;
        i++;
    }
}

void listT_remove(listT *list, int pos) {}

void *listT_get(listT *list, int pos) {
    if (list->size <= pos)
        return NULL;
    if (list->begin == NULL)
        return NULL;

    nodeT *curr = list->begin;
    for (int i = 0; i < pos; i++) {
        curr = curr->next;
    }

    return curr->value;
}
