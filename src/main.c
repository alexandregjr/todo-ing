#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void checkout(int n) {
    printf("\x1b[2J\x1b[H");
    exit(0);
}

int main(void) {
    void *temp;
    struct sigaction act;

    temp = memset(&act, 0, sizeof(struct sigaction));
    if (!temp) {
        exit(1);
    }

    act.sa_handler = checkout;

    int res;
    res = sigaction(SIGINT, &act, NULL);
    if (res < 0) {
        exit(2);
    }

    system("stty raw");
    char in = 0;
    while (1) {
        // Render
        printf("\x1b[2J\x1b[H"); // ESC code to clear screen & move cursor to
                                 // top left
        printf("Hello World! char=(%c, %d)\n", in, in);
        printf("\x1b[2H. to exit\n");
        printf("\x1b[3H");

        // Get input
        in = getchar();
        if (in == '.') {
            system("stty cooked");
            printf("\x1b[2J\x1b[H");
            exit(0);
        }
    }

    return 0;
}
