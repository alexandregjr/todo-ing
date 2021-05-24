#ifndef COMMAND_H_
#define COMMAND_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "todo.h"

void parse_command(char *, int, listT *);
void save_in_disk(char **, listT *);
void load_from_disk(char **, listT *);
void quit(char **args, listT *);

#endif // COMMAND_H_
