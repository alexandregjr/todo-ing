#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "list.h"
#include "text_utils.h"
#include "todo.h"

typedef struct {
    int r;
    int c;
} posT;

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

    i = trimn(str, i < len ? i : len);

    return i;
}

void render_command_line(char *str) {
    printf(LAST_LINE FONT_BOLD FONT_WHITE ":%s", str);
}

void parse_command(char *cmd, int len, listT* todo_list) {
    if (!strncmp(cmd, "q", len)) {
        quit(NULL, todo_list);
    }

    if (!strncmp(cmd, "w", len)) {
        save_in_disk(NULL, todo_list);
    }

    if (len < 2)
        return;

    else if (!strncmp(cmd, "wq", len)) {
        save_in_disk(NULL, todo_list);
        quit(NULL, todo_list);
    }
}

void get_command(listT* todo_list) {
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

    parse_command(command, i, todo_list);
}

void render(listT* todo_list, posT cursor) {
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
// TODO(#8): support for different workspaces
//   it should work like having multiple files open in vim, but with workspace
//   bar in the bottom (like i3 for instance)
int main(void) {

    char in = 0;
    char str[1024];

    posT cursor = {0, 0};

    listT *todo_list = (listT *)calloc(1, sizeof(listT));
    todoT *selected = NULL;
    
    load_from_disk(NULL, todo_list);
    if (todo_list->size > 0)
        selected = listT_get(todo_list, cursor.r);

    system("stty raw");
    while (1) {
        render(todo_list, cursor);

        // Get input
        in = getchar();
        switch (in) {
        case ':':
            render(todo_list, cursor);
            get_command(todo_list);
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
            if (len == 0)
                break;

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
        }
    }

    return 0;
}
