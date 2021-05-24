#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "io.h"
#include "list.h"
#include "todo.h"

// TODO(#4): add different lists for each todo state
// TODO(#5): add vim-like commands
// TODO(#6): add color/font-style support
//  (https://en.wikipedia.org/wiki/ANSI_escape_code#SGR)
// TODO(#8): support for different workspaces
//   it should work like having multiple files open in vim, but with workspace
//   bar in the bottom (like i3 for instance)
// TODO(#9): add pre-compiled headers
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
