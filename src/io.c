#include "io.h"

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

void get_command(listT *todo_list) {
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

void render_command_line(char *str) {
    printf(LAST_LINE FONT_BOLD FONT_WHITE ":%s", str);
}

void render(listT *todo_list, posT cursor) {
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
