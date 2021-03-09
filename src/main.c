#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void checkout(int n) {
    printf("\x1b[2J\x1b[H");
    exit(1);
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

    while (1) {
        printf("\x1b[H"); // ESC code to move cursor to top left
        printf("Hello World!\n");
        printf("\x1B[2J"); // ESC code to clear screen
    }

    return 0;
}
