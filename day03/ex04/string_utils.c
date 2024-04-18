#include "main.h"

char is_printable(char c) { return c >= 32 && c <= 126; }

bool str_equal(const char* s1, const char* s2) {
    while (*s1 || *s2) {
        if (*s1++ != *s2++) return false;
    }
    return true;
}