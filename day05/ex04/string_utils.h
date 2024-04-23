#pragma once

#include "main.h"

char ft_isprint(char c);
char ft_isdigit(char c);
size_t ft_strlen(char* s);
void ft_strcpy(char* dst, const char* src);
bool str_is_space(char* s);
bool str_equals(char* s1, char* s2);
size_t hash(uint8_t* str);
