#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define LED (1 << PB0)
#define PIN (1 << PD2)

#define DEBOUNCE_ITERATIONS 12

volatile bool button_is_pressed = false;

uint8_t min(uint8_t a, uint8_t b) { return a < b ? a : b; }

int main() {
    DDRB = LED;
    DDRD &= ~PIN;
    PORTD |= PIN;

    EIMSK |= 1 << INT0;
    EICRA = 0b10;

    sei();

    uint8_t counter = 0;
    while (true) {
        counter = button_is_pressed ? min(counter, DEBOUNCE_ITERATIONS) + 1 : 0;
        if (counter == DEBOUNCE_ITERATIONS) PORTB ^= LED;
        _delay_ms(1);
    }
}

ISR(INT0_vect) {
    button_is_pressed = EICRA == 0b10;
    EICRA ^= 0b01;
}
