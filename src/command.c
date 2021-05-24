#include "command.h"

void parse_command(char *cmd, int len, listT *todo_list) {
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

void _item_writer(void *todo, FILE *f) {
    fprintf(f, "%d\t%d\t%s", ((todoT *)todo)->progress, ((todoT *)todo)->len,
            ((todoT *)todo)->text);
}

void _item_reader(void **todo, FILE *f) {
    int state, len;
    fscanf(f, "%d\t%d\t", &state, &len);
    char str[len + 1];
    fgets(str, len + 1, f);
    *todo = create_todo(str, len, state);
}

void save_in_disk(char **args, listT *todo_list) {
    char save_path[1024];
    snprintf(save_path, 1024, "%s", getenv("HOME"));
    strncat(save_path, "/.config/todo-ing.db", 1024);
    listT_write(todo_list, save_path, _item_writer);
}

void load_from_disk(char **args, listT *todo_list) {
    char load_path[1024];
    snprintf(load_path, 1024, "%s", getenv("HOME"));
    strncat(load_path, "/.config/todo-ing.db", 1024);
    listT_load(todo_list, load_path, _item_reader);
}

void quit(char **args, listT *todo_list) {
    system("stty cooked");
    printf("\x1b[2J\x1b[H");
    exit(0);
}
