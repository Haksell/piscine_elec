#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define DEBOUNCE_ITERATIONS 20

int main() {
    DDRB = 1 << PB0;
    int counter = 0;
    while (true) {
        bool is_released = (PIND >> PD2) & 1;
        counter = is_released ? 0 : counter > DEBOUNCE_ITERATIONS ? counter : counter + 1;
        if (counter == DEBOUNCE_ITERATIONS) { PORTB ^= 1 << PB0; }
        _delay_ms(1);
    }
}

// PCINT2 PCINT2_vect
