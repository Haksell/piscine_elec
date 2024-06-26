#include "main.h"

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

void uart_printfloat(float x, int precision) {
    char s[16];
    dtostrf(x, 0, precision, s);
    uart_printstr(s);
}

void uart_printstr(const char* str) {
    while (*str) uart_tx(*str++);
}

void uart_printstrln(const char* str) {
    uart_printstr(str);
    uart_printstr("\r\n");
}

void uart_print_nibble(uint8_t byte) { uart_tx(byte < 10 ? byte + '0' : byte + 55); }

void print_hex_value(uint8_t byte) {
    uart_print_nibble(byte >> 4);
    uart_print_nibble(byte & 15);
    uart_tx(' ');
}
