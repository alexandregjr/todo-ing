#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "utils.h"

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

listT *todo_list;
char save_path[1024];

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
    if (((todoT *)todo)->progress == 0) {
        printf("\x1b[%dH" BG_RED "  " RESET_DECOR FONT_WHITE " %s", pos + 2,
               ((todoT *)todo)->text);
    } else if (((todoT *)todo)->progress == 1) {
        printf("\x1b[%dH" BG_YELLOW "  " RESET_DECOR FONT_WHITE " %s", pos + 2,
               ((todoT *)todo)->text);
    } else if (((todoT *)todo)->progress == 2) {
        printf("\x1b[%dH" BG_GREEN "  " RESET_DECOR FONT_WHITE" %s", pos + 2,
               ((todoT *)todo)->text);
    }
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

void render_command_line(char *str) {
    printf(LAST_LINE FONT_BOLD FONT_WHITE ":%s", str);
}

void save_todos() { listT_write(todo_list, save_path, todo_writer); }

void quit() {
    system("stty cooked");
    printf("\x1b[2J\x1b[H");
    exit(0);
}

void parse_command(char *cmd, int len) {
    if (!strncmp(cmd, "q", len)) {
        quit();
    }

    if (!strncmp(cmd, "w", len)) {
        save_todos();
    }

    if (len < 2) return;

    else if (!strncmp(cmd, "wq", len)) {
        save_todos();
        quit();
    }
}

void get_command() {
    char command[1024];
    memset(command, 0, sizeof(command));

    int i = 0;
    char in = 0;
    while (in != 13 && i < 1024) {
        render_command_line(command);

        in = getchar();
        if (in == 127) {
            command[--i] = '\0';
            continue;
        }

        if (in == 13)
            break;

        command[i++] = in;
    }

    if (i < 1024)
        command[i] = '\0';

    parse_command(command, i);
}

void render(posT cursor) {
    // Render
    // top left
    printf("\x1b[2J\x1b[H" FONT_BOLD FONT_ITALIC FONT_GREEN
           "todo-ing app" RESET_DECOR);

    // Render Todo's
    listT_print(todo_list, print_todo);

    // Set cursor pos
    if (todo_list->size > 0) {
        printf("\x1b[%d;%dH", 2 + cursor.r, 5 + cursor.c);
    } else {
        printf("\x1b[H");
    }
}

// TODO(#4): add different lists for each todo state
// TODO(#5): add vim-like commands
// TODO(#6): add color/font-style support
//  (https://en.wikipedia.org/wiki/ANSI_escape_code#SGR)
int main(void) {

    snprintf(save_path, 1024, "%s", getenv("HOME"));
    strncat(save_path, "/.config/todo-ing.db", 1024);

    char in = 0;
    char str[1024];

    posT cursor = {0, 0};

    todo_list = (listT *)calloc(1, sizeof(listT));
    todoT *selected = NULL;

    listT_load(todo_list, save_path, todo_reader);
    if (todo_list->size > 0)
        selected = listT_get(todo_list, cursor.r);

    system("stty raw");
    while (1) {
        render(cursor);

        // Get input
        in = getchar();
        switch (in) {
        case ':':
            render(cursor);
            get_command();
            break;
        // case '.':
        //    break;
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
            // case 's':
            //    listT_write(todo_list, save_path, todo_writer);
            //    break;
        }
    }

    return 0;
}
