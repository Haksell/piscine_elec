#pragma once

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include <util/twi.h>

#define USERNAME "spectre"
#define PASSWORD "oogachaka"

#define BACKSPACE '\x7f'
#define CRLF "\r\n"

#define ROUND_DIV(dividend, divisor)                                                               \
    ({                                                                                             \
        typeof(divisor) _divisor = (divisor);                                                      \
        ((dividend) + (_divisor >> 1)) / _divisor;                                                 \
    })

// uart.c
void uart_init();
void uart_tx(char c);
char uart_rx();
void uart_printstr(const char* str);
void uart_printstrln(const char* str);
