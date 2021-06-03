#include "io.h"

int get_input(char *str, int len) {
    memset(str, 0, len);

    int i = 0;
    char in = 0;

    while (in != 13 && i < len) {
        clear_screen();
        set_cursor(1, 1);
        print_formatted(FONT_WHITE FONT_BOLD, "todo:");
        if (i > 0) {
            set_cursor(1, 7);
            print_formatted(FONT_WHITE, "%s", str);
        }

        in = getchar();
        if (in == 127) {
            if (i > 0)
                str[--i] = '\0';

            continue;
        }

        if (in == 3) {
            system("stty cooked");
            clear_screen();
            set_cursor(1, 1);
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
    move_cursor(10000, 1);
    clear_line();
    move_cursor(0, -10000);
    print_formatted(COMMAND_STYLE, ":%s", str);
}

void render(listT *todo_list, posT cursor) {
    // Render
    // top left
    clear_screen();
    set_cursor(1, 1);
    print_formatted(TITLE_STYLE, "todo-ing app");

    // Render Todo's
    listT_print(todo_list, print_todo);

    // Set cursor pos
    if (todo_list->size > 0) {
        set_cursor(2 + cursor.r, 5 + cursor.c);
    } else {
        set_cursor(1, 1);
    }
}
