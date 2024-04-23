#include "main.h"

char ft_isprint(char c) { return 32 <= c && c <= 126; }

char ft_isdigit(char c) { return '0' <= c && c <= '9'; }

size_t ft_strlen(char* s) {
    size_t i = 0;
    while (s[i]) ++i;
    return i;
}

bool str_is_space(char* s) {
    while (*s) {
        if (*s != ' ') return false;
        ++s;
    }
    return true;
}

bool str_equals(char* s1, char* s2) {
    while (*s1 || *s2) {
        if (*s1++ != *s2++) return false;
    }
    return true;
}

#if DEBUG
size_t hash(uint8_t* str) {
    size_t res = 0;
    while (*str) {
        res += *str;
        ++str;
    }
    return res;
}
#else
size_t hash(uint8_t* str) {
    size_t res = 5381;
    while (*str) {
        res = res << 5 + res + *str;
        ++str;
    }
    return res;
}
#endif
