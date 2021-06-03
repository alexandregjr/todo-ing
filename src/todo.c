#include "todo.h"

todoT *create_todo(char *text, int len, enum state init) {
    todoT *todo = (todoT *)calloc(1, sizeof(todoT));

    todo->progress = init;
    todo->text = (char *)calloc(len + 1, sizeof(char));
    strcpy(todo->text, text);
    todo->len = len;

    return todo;
}

void print_todo(void *todo, int pos) {
    set_cursor(pos + 2, 1);
    if (((todoT *)todo)->progress == 0) {
        print_formatted(TODO_COLOR, "  ");
    } else if (((todoT *)todo)->progress == 1) {
        print_formatted(DOING_COLOR, "  ");
    } else if (((todoT *)todo)->progress == 2) {
        print_formatted(DONE_COLOR, "  ");
    }
    print_formatted(TEXT_STYLE, " %s", ((todoT *)todo)->text);
}

void destroy_todo(void *todo) {
    free(((todoT *)todo)->text);
    free((todoT *)todo);
}
