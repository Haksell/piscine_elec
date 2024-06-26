#pragma once

#include "main.h"

#define BACKSPACE '\x7f'

void uart_init();
void uart_tx(char c);
char uart_rx();
void uart_newline();
void uart_putstr(const char* str);
void uart_putstrln(const char* str);
void uart_putnbr(uint32_t n);
void uart_putnbrln(uint32_t n);
void uart_putbyte(uint8_t n);
void uart_readline(char* buffer, size_t buffer_size);