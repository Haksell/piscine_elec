#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

volatile uint8_t counter = 13; // Start value to prove output works. TODO: set at 0

int main() {
    sei();

    DDRB = 0b10111;
    DDRD &= ~(1 << PD2);
    PORTD |= 1 << PORTD2;
    EIMSK |= 1 << INT0;
    PCMSK2 = 1 << PCINT18;
    EICRA = 0b00;
    EIFR |= (1 << INTF0);

    while (true) {
        PORTB = (counter & 7) | (counter & 8) << 1;
        _delay_ms(10);
    }
}

ISR(INT0_vect) { ++counter; }
