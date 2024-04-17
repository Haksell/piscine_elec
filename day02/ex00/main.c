#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define DEBOUNCE_ITERATIONS 12

volatile uint8_t counter = 13; // Start value to prove output works. TODO: set at 0

int main() {
    sei();

    DDRB = 0b10111;
    EIMSK = 1 << INT0;
    PCMSK2 = 1 << PCINT18;
    EICRA = 0b00;

    while (true) {
        PORTB = (counter & 7) | (counter & 8) << 1;
        _delay_ms(1);
    }
}

ISR(INT0_vect) { ++counter; }
