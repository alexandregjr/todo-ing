#ifndef TODO_H_
#define TODO_H_

enum state { TODO = 0, DOING, DONE };

typedef struct {
    char *text;
    int len;
    enum state progress;
} todoT;

todoT *create_todo(char *, int, enum state);
void print_todo(void *, int );
void destroy_todo(void *);

#endif // TODO_H_
