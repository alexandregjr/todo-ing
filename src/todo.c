#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "todo.h"
#include "text_utils.h"

todoT *create_todo(char *text, int len, enum state init) {
    todoT *todo = (todoT *)calloc(1, sizeof(todoT));

    todo->progress = init;
    todo->text = (char *)calloc(len + 1, sizeof(char));
    strcpy(todo->text, text);
    todo->len = len;

    return todo;
}

void print_todo(void *todo, int pos) {
    if (((todoT *)todo)->progress == 0) {
        printf("\x1b[%dH" BG_RED "  " RESET_DECOR FONT_WHITE " %s", pos + 2,
               ((todoT *)todo)->text);
    } else if (((todoT *)todo)->progress == 1) {
        printf("\x1b[%dH" BG_YELLOW "  " RESET_DECOR FONT_WHITE " %s", pos + 2,
               ((todoT *)todo)->text);
    } else if (((todoT *)todo)->progress == 2) {
        printf("\x1b[%dH" BG_GREEN "  " RESET_DECOR FONT_WHITE " %s", pos + 2,
               ((todoT *)todo)->text);
    }
}

void destroy_todo(void *todo) {
    free(((todoT *)todo)->text);
    free((todoT *)todo);
}
