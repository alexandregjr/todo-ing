#ifndef LIST_H_
#define LIST_H_

typedef struct node {
    void *value;
    struct node *next;
} nodeT;

typedef struct {
    nodeT *begin;
    int size;
} listT;

void listT_push(listT *, void *);
void listT_print(listT *, void (*)(void *, int));
void listT_remove(listT *, int, void (*)(void *));
void listT_write(listT *, char *, void (*)(void *, FILE *));
void listT_load(listT *, char *, void (*)(void **, FILE *));
void *listT_get(listT *, int);

#endif // LIST_H_
