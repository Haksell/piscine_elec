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

void uart_printstr(const char* str) {
    while (*str) uart_tx(*str++);
}

void uart_printstrln(const char* str) {
    uart_printstr(str);
    uart_printstr(CRLF);
}
