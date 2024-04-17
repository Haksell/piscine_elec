#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define LED (1 << PB0)
#define BUTTON (1 << PD2)

#define DEBOUNCE_ITERATIONS 7

volatile int8_t debounce_counter = -1;

ISR(INT0_vect) {
    debounce_counter = EICRA == 0b10 ? DEBOUNCE_ITERATIONS : -1;
    EICRA ^= 0b01;
}

int main() {
    DDRB |= LED;
    DDRD &= ~BUTTON;
    PORTD |= BUTTON;

    EIMSK |= 1 << INT0;
    EICRA = 0b10;
    sei();

    while (true) {
        if (debounce_counter >= 0) --debounce_counter;
        if (debounce_counter == 0) PORTB ^= LED;
        _delay_ms(1);
    }

    return 0;
}
