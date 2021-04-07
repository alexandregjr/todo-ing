#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

todoT *create_todo(char *text, int len) {
    todoT *todo = (todoT *)calloc(1, sizeof(todoT));

    todo->progress = TODO;
    todo->text = (char *)calloc(len + 1, sizeof(char));
    strcpy(todo->text, text);
    todo->len = len;

    return todo;
}

int get_input(char *str, int len) {
    int i = 0;
    char in = 0;
    // TODO: backspace key erase last char
    while (in != 13 && i < len) {
        in = getchar();
        if (in == 3) {
            system("stty cooked");
            printf("\x1b[2J\x1b[H");
            exit(0);
            break;
        }
        str[i] = in;
        i++;
    }

    if (i < len)
        str[i] = 0;

    return i;
}

int main(void) {
    system("stty raw");

    char in = 0;
    char str[1024];

    posT cursor = {0, 0};

    todoT **todo_list = (todoT **)calloc(1024, sizeof(todoT *));
    int todo_count = 0;
    while (1) {
        // Render
        printf("\x1b[2J\x1b[H"); // ESC code to clear screen & move cursor to
                                 // top left
        printf("Hello World! char=(%c, %d)\n", in, in);

        // RENDER TODOS
        for (int i = 0; i < todo_count; i++) {
            printf("\x1b[%dH[%d] %s\n", 2 + i, todo_list[i]->progress,
                   todo_list[i]->text);
        }
        // END RENDER TODOS;

        printf("\x1b[%dH. to exit\n", 3 + todo_count);

        if (todo_count > 0) {
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
        case 'a':
            printf("\x1b[2J\x1b[Htodo: ");
            int len = get_input(str, 1024);
            todoT *new_todo = create_todo(str, len);
            todo_list[todo_count++] = new_todo;
            break;
        case 'h':
            if (todo_count == 0)
                break;
            if (cursor.c > 0)
                cursor.c--;
            break;
        case 'j':
            if (todo_count == 0)
                break;
            if (cursor.r < todo_count - 1)
                cursor.r++;
            if (cursor.c > todo_list[cursor.r]->len - 2)
                cursor.c = todo_list[cursor.r]->len - 2;
            break;
        case 'k':
            if (todo_count == 0)
                break;
            if (cursor.r > 0)
                cursor.r--;
            if (cursor.c > todo_list[cursor.r]->len - 2)
                cursor.c = todo_list[cursor.r]->len - 2;
            break;
        case 'l':
            if (todo_count == 0)
                break;
            if (cursor.c < todo_list[cursor.r]->len - 2)
                cursor.c++;
            break;
        case 'd':
            if (todo_count == 0)
                break;
            todo_list[cursor.r]->progress = DOING;
            break;
        case 'f':
            if (todo_count == 0)
                break;
            todo_list[cursor.r]->progress = DONE;
            break;
        case 't':
            if (todo_count == 0)
                break;
            todo_list[cursor.r]->progress = TODO;
            break;
        }
    }

    return 0;
}
