#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

static void uart_init(void) {
    UBRR0 = F_CPU / 8 / UART_BAUDRATE - 1; // Set baud rate register
    UCSR0A |= 1 << U2X0; // Enable 2x speed
    UCSR0B |= 1 << TXEN0; // Enable transmitter
}

static void uart_tx(char c) {
    while (!(UCSR0A & 1 << UDRE0)) {} // Wait for the transmit buffer to be empty
    UDR0 = c; // Put data into the buffer
}

int main() {
    uart_init();
    while (true) {
        uart_tx('Z');
        _delay_ms(1000);
    }
}
