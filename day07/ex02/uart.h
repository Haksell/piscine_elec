#pragma once

#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>

#define ROUND_DIV(dividend, divisor)                                                               \
    ({                                                                                             \
        typeof(divisor) _divisor = (divisor);                                                      \
        ((dividend) + (_divisor >> 1)) / _divisor;                                                 \
    })

#define BACKSPACE '\x7f'
#define CRLF "\r\n"

void uart_init();
void uart_tx(char c);
char uart_rx();
void uart_newline();
void uart_putstr(const char* str);
void uart_putstrln(const char* str);
void uart_putnbr(uint32_t n);
void uart_putnbrln(uint32_t n);
void uart_putbyte(uint8_t n);
void uart_putfloat(float x, int precision);
void uart_readline(char* buffer, size_t buffer_size);
