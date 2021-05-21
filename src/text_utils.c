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
