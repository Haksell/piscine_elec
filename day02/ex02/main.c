#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define BUTTONS (1 << PD2 | 1 << PD4)

#define DEBOUNCE_ITERATIONS 7

volatile int8_t counterSW1 = -1;
volatile int8_t n = 0;

ISR(INT0_vect) {
    counterSW1 = EICRA == 0b10 ? DEBOUNCE_ITERATIONS : -1;
    EICRA ^= 0b01;
}

int main() {
    DDRB = 0b10111;
    DDRD &= ~BUTTONS;
    PORTD |= BUTTONS;

    EIMSK |= 1 << INT0;
    EICRA = 0b10;
    sei();

    while (true) {
        if (counterSW1 >= 0) --counterSW1;
        if (counterSW1 == 0) ++n;
        PORTB = (n & 7) | (n & 8) << 1;
        _delay_ms(1);
    }

    return 0;
}
