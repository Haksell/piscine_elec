#pragma once

#include "main.h"

void uart_init();
void uart_tx(char c);
char uart_rx();
void uart_putnbr(uint32_t n);
void uart_putnbrln(uint32_t n);
void uart_putfloat(float x, int precision);
void uart_putstr(const char* str);
void uart_putstrln(const char* str);
