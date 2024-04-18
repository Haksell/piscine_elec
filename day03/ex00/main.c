#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

static void uart_init(void) {
    UBRR0 = F_CPU / 8 / UART_BAUDRATE - 1; // set baud rate register
    UCSR0A |= 1 << U2X0; // enable 2x speed
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
