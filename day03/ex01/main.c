#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define ROUND_DIV(dividend, divisor)                                                               \
    ({                                                                                             \
        typeof(divisor) _divisor = (divisor);                                                      \
        ((dividend) + (_divisor >> 1)) / _divisor;                                                 \
    })

static void uart_init(void) {
    UBRR0 = ROUND_DIV(F_CPU, 16 * UART_BAUDRATE) - 1;
    UCSR0B |= 1 << TXEN0;
}

static void uart_tx(char c) {
    while (!(UCSR0A & 1 << UDRE0)) {}
    UDR0 = c;
}

static void uart_printstr(const char* str) {
    while (*str) uart_tx(*str++);
}

static void uart_printstrln(const char* str) {
    uart_printstr(str);
    uart_tx('\r');
    uart_tx('\n');
}

int main() {
    uart_init();
    while (true) {
        uart_printstrln("Hello World!");
        _delay_ms(2000);
    }
}
