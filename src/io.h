#ifndef INPUT_OUTPUT_H_
#define INPUT_OUTPUT_H_

#include <stdio.h>
#include <string.h>

#include "command.h"
#include "text_utils.h"
#include "list.h"
#include "todo.h"

typedef struct {
    int r;
    int c;
} posT;

int get_input(char *, int);
void get_command(listT *);
void render_command_line(char *);
void render(listT *, posT);

#endif // INPUT_OUTPUT_H_
