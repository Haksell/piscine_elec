#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>

#define ROUND_DIV(dividend, divisor)                                                               \
    ({                                                                                             \
        typeof(divisor) _divisor = (divisor);                                                      \
        ((dividend) + (_divisor >> 1)) / _divisor;                                                 \
    })

static char swapcase(char c) {
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') ? c ^ 32 : c;
}

static void uart_init() {
    UBRR0 = ROUND_DIV(F_CPU, 16 * UART_BAUDRATE) - 1;
    UCSR0B |= 1 << RXCIE0 | 1 << RXEN0 | 1 << TXEN0;
}

static void uart_tx(char c) {
    while (!(UCSR0A & 1 << UDRE0)) {}
    UDR0 = c;
}

static char uart_rx() {
    while (!(UCSR0A & 1 << RXC0)) {}
    return UDR0;
}

ISR(USART_RX_vect) { uart_tx(swapcase(uart_rx())); }

int main() {
    uart_init();
    sei();
    while (true) {}
}
