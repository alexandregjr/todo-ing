#ifndef UTILS_H_
#define UTILS_H_

// DECORATIONS
#define FONT_BLACK "\x1b[30m"
#define FONT_RED "\x1b[31m"
#define FONT_GREEN "\x1b[32m"
#define FONT_YELLOW "\x1b[33m"
#define FONT_BLUE "\x1b[34m"
#define FONT_MAGENTA "\x1b[35m"
#define FONT_CYAN "\x1b[36m"
#define FONT_WHITE "\x1b[37m"

#define BG_BLACK "\x1b[40m"
#define BG_RED "\x1b[41m"
#define BG_GREEN "\x1b[42m"
#define BG_YELLOW "\x1b[43m"
#define BG_BLUE "\x1b[44m"
#define BG_MAGENTA "\x1b[45m"
#define BG_CYAN "\x1b[46m"
#define BG_WHITE "\x1b[47m"

#define FONT_BOLD "\x1b[1m"
#define FONT_ITALIC "\x1b[3m"

#define RESET_DECOR "\x1b[m"
// END DECORATIONS

// MOVEMENTS
#define LAST_LINE "\x1b[10000B\x1b[2K\x1b[G"
// END MOVEMENTS

#endif // UTILS_H_
