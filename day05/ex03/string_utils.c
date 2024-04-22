#include "main.h"

char ft_isprint(char c) { return 32 <= c && c <= 126; }

char ft_isdigit(char c) { return '0' <= c && c <= '9'; }

size_t ft_strlen(char* s) {
    size_t i = 0;
    while (s[i]) ++i;
    return i;
}