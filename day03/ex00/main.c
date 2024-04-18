#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define ROUND_DIV(dividend, divisor) (((dividend) + ((divisor) >> 1)) / (divisor))

static void uart_init(void) {
    UBRR0 = ROUND_DIV(F_CPU, 16 * UART_BAUDRATE) - 1; // set baud rate register
    UCSR0B |= 1 << TXEN0; // enable transmitter
    // we could set 8N1 format in UCSR0B but it seems to be the default already
}

static void uart_tx(char c) {
    while (!(UCSR0A & 1 << UDRE0)) {} // wait for the transmit buffer to be empty
    UDR0 = c; // put data into the buffer
}

int main() {
    uart_init();
    while (true) {
        uart_tx('Z');
        _delay_ms(1000);
    }
}
