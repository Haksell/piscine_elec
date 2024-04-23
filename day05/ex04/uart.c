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

void uart_newline() {
    uart_tx('\r');
    uart_tx('\n');
}

void uart_putstr(const char* str) {
    while (*str) uart_tx(*str++);
}

void uart_putstrln(const char* str) {
    uart_putstr(str);
    uart_newline();
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
    uart_newline();
}

static void uart_putnibble(uint8_t n) { uart_tx(n + (n < 10 ? 48 : 87)); }

void uart_putbyte(uint8_t n) {
    uart_putnibble(n >> 4);
    uart_putnibble(n & 15);
}

void uart_readline(char* buffer, size_t buffer_size) {
    size_t i = 0;
    uart_putstr("> ");
    while (true) {
        char c = uart_rx();
        if (c == '\r') {
            buffer[i] = '\0';
            uart_newline();
            return;
        } else if (c == BACKSPACE && i > 0) {
            --i;
            uart_putstr("\b \b");
        } else if (ft_isprint(c) && i < buffer_size - 1) {
            buffer[i] = c;
            ++i;
            uart_tx(c);
        }
    }
}
