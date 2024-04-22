#include "main.h"

#define CRLF "\r\n"

void uart_init() {
    UBRR0 = ROUND_DIV(F_CPU, 16 * UART_BAUDRATE) - 1;
    UCSR0B |= 1 << RXEN0 | 1 << TXEN0;
}

void uart_tx(char c) {
    while (!(UCSR0A & 1 << UDRE0)) {}
    UDR0 = c;
}

char uart_rx() {
    while (!(UCSR0A & 1 << RXC0)) {}
    return UDR0;
}

void uart_putnbr(uint32_t n) {
    if (n < 10) uart_tx(n + '0');
    else {
        uart_putnbr(n / 10);
        uart_tx(n % 10 + '0');
    }
}

void uart_putnbrln(uint32_t n) {
    uart_putnbr(n);
    uart_putstr(CRLF);
}

void uart_putfloat(float x, int precision) {
    char s[16];
    dtostrf(x, 0, precision, s);
    uart_putstr(s);
}

void uart_putstr(const char* str) {
    while (*str) uart_tx(*str++);
}

void uart_putstrln(const char* str) {
    uart_putstr(str);
    uart_putstr(CRLF);
}
