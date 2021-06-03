#ifndef TODO_H_
#define TODO_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "text_utils.h"

#define TEXT_STYLE FONT_WHITE
#define TODO_COLOR BG_RED
#define DOING_COLOR BG_YELLOW
#define DONE_COLOR BG_GREEN

enum state { TODO = 0, DOING, DONE };

typedef struct {
    char *text;
    int len;
    enum state progress;
} todoT;

todoT *create_todo(char *, int, enum state);
void print_todo(void *, int);
void destroy_todo(void *);

#endif // TODO_H_
