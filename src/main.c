#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_input(char *str, int len) {
    int i = 0;
    char in = 0;
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
    while (1) {
        // Render
        printf("\x1b[2J\x1b[H"); // ESC code to clear screen & move cursor to
                                 // top left
        printf("Hello World! char=(%c, %d)\n", in, in);
        printf("\x1b[2H. to exit\n");
        printf("\x1b[3H");

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
            get_input(str, 1024);

            break;
        }
    }

    return 0;
}
