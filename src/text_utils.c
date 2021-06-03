#include "text_utils.h"

int trimn(char *str, int len) {
    int l_count = 0;
    while (l_count < len && (str[l_count] == ' ' || str[l_count] == '\t')) {
        l_count++;
    }

    if (l_count == len) {
        str[0] = '\0';
        return 0;
    }

    for (int i = 0; i < len - l_count; i++) {
        str[i] = str[i + l_count];
    }

    str[len - l_count] = '\0';

    len -= l_count;
    int r_count = 0;
    while (len - 1 - r_count >= 0 &&
           (str[len - 1 - r_count] == ' ' || str[len - 1 - r_count] == '\t')) {
        r_count++;
    }

    str[len - r_count] = '\0';

    return len - r_count;
}

void print_formatted(char* format, char* text, ...) {
    va_list args;
    va_start(args, text);
    
    printf("%s", format);
    vprintf(text, args);
    printf("%s", RESET_DECOR);
}

void set_cursor(int x, int y) {
    printf("\x1b[%d;%dH", x, y);
}

void move_cursor(int x, int y) {
    if (x < 0) {
        printf("\x1b[%dA", x - (2 * x));
    } else if (x > 0) {
        printf("\x1b[%dB", x);
    }

    if (y < 0) {
        printf("\x1b[%dD", y - (2 * y));
    } else if (y > 0) {
        printf("\x1b[%dC", y);
    }
}

void clear_screen() {
    printf("\x1b[2J");
}

void clear_line() {
    printf("\x1b[2K");
}
