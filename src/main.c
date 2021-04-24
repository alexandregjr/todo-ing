#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

enum state { TODO = 0, DOING, DONE };

typedef struct {
    char *text;
    int len;
    enum state progress;
} todoT;

typedef struct {
    int r;
    int c;
} posT;

todoT *create_todo(char *text, int len, enum state init) {
    todoT *todo = (todoT *)calloc(1, sizeof(todoT));

    todo->progress = init;
    todo->text = (char *)calloc(len + 1, sizeof(char));
    strcpy(todo->text, text);
    todo->len = len;

    return todo;
}

int get_input(char *str, int len) {
    memset(str, 0, len);

    int i = 0;
    char in = 0;

    while (in != 13 && i < len) {
        printf("\x1b[2J\x1b[Htodo:");
        if (i > 0) {
            printf("\x1b[1;7H%s", str);
        }

        in = getchar();
        if (in == 127) {
            str[--i] = '\0';
            continue;
        }
        if (in == 3) {
            system("stty cooked");
            printf("\x1b[2J\x1b[H");
            exit(0);
            break;
        }
        if (in == 13)
            break;

        str[i++] = in;
    }

    if (i < len)
        str[i] = '\0';

    return i;
}

void print_todo(void *todo, int pos) {
    printf("\x1b[%dH[%d] %s", pos + 2, ((todoT *)todo)->progress,
           ((todoT *)todo)->text);
}

void todo_writer(void *todo, FILE *f) {
    fprintf(f, "%d\t%d\t%s", ((todoT *)todo)->progress, ((todoT *)todo)->len,
            ((todoT *)todo)->text);
}

void todo_reader(void **todo, FILE *f) {
    int state, len;
    fscanf(f, "%d\t%d\t", &state, &len);
    char str[1024];
    fgets(str, 1025, f);
    *todo = create_todo(str, len, state);
}

void destroy_todo(void *todo) {
    free(((todoT *)todo)->text);
    free((todoT *)todo);
}

// TODO(#4): add different lists for each todo state
// TODO(#5): add vim-like commands
int main(void) {

    char save_path[1024];
    snprintf(save_path, 1024, "%s", getenv("HOME"));
    strncat(save_path, "/.config/todo-ing.db", 1024);

    char in = 0;
    char str[1024];

    posT cursor = {0, 0};

    listT *todo_list = (listT *)calloc(1, sizeof(listT));
    todoT *selected = NULL;

    listT_load(todo_list, save_path, todo_reader);
    if (todo_list->size > 0)
        selected = listT_get(todo_list, cursor.r);

    while (1) {
        system("stty raw");
        // Render
        printf("\x1b[2J\x1b[H"); // ESC code to clear screen & move cursor to
                                 // top left
        printf("Hello World! char=(%c, %d)\n", in, in);

        // Render Todo's
        listT_print(todo_list, print_todo);

        printf("\x1b[%dH. to exit\n", 3 + todo_list->size);

        if (todo_list->size > 0) {
            printf("\x1b[%d;%dH", 2 + cursor.r, 5 + cursor.c);
        } else {
            printf("\x1b[H");
        }

        // Get input
        in = getchar();
        switch (in) {
        case '.':
            system("stty cooked");
            printf("\x1b[2J\x1b[H");
            exit(0);
            break;
        case 'r':
            if (todo_list->size == 0)
                break;
            listT_remove(todo_list, cursor.r, destroy_todo);
            if (cursor.r >= todo_list->size)
                cursor.r = todo_list->size > 0 ? todo_list->size - 1 : 0;
            selected = listT_get(todo_list, cursor.r);
            break;
        case 'a':
            printf("\x1b[2J\x1b[Htodo: ");
            int len = get_input(str, 1024);
            todoT *new_todo = create_todo(str, len, TODO);
            listT_push(todo_list, (void *)new_todo);
            selected = new_todo;
            break;
        case 'h':
            if (todo_list->size == 0)
                break;
            if (cursor.c > 0)
                cursor.c--;
            break;
        case 'j':
            if (todo_list->size == 0)
                break;
            if (cursor.r < todo_list->size - 1) {
                cursor.r++;
                selected = (todoT *)listT_get(todo_list, cursor.r);
            }
            if (cursor.c > selected->len - 2)
                cursor.c = selected->len - 2;
            break;
        case 'k':
            if (todo_list->size == 0 || selected == NULL)
                break;
            if (cursor.r > 0) {
                cursor.r--;
                selected = (todoT *)listT_get(todo_list, cursor.r);
            }
            if (cursor.c > selected->len - 2)
                cursor.c = selected->len - 2;
            break;
        case 'l':
            if (todo_list->size == 0 || selected == NULL)
                break;
            if (cursor.c < selected->len - 2)
                cursor.c++;
            break;
        case 'd':
            if (todo_list->size == 0)
                break;
            selected->progress = DOING;
            break;
        case 'f':
            if (todo_list->size == 0)
                break;
            selected->progress = DONE;
            break;
        case 't':
            if (todo_list->size == 0)
                break;
            selected->progress = TODO;
            break;
        case 's':
            listT_write(todo_list, save_path, todo_writer);
            break;
        }
    }

    return 0;
}
