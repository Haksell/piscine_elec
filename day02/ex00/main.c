#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

volatile uint8_t counter = 0; // Start value to prove output works. TODO: set at 0

int main() {
    sei();

    DDRB = 0b10111;
    EIMSK = 1 << INT0;
    EICRA = 0b10;

    while (true) {
        PORTB = (counter & 7) | (counter & 8) << 1;
        _delay_ms(10);
    }
}

ISR(INT0_vect) {
    EICRA ^= 0b11;
    // millis to debounce?
    ++counter;
}
