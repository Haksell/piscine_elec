#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define BUTTONS (1 << PD2 | 1 << PD4)
#define DEBOUNCE_ITERATIONS 7

volatile int8_t counterSW1 = -1;
volatile int8_t counterSW2 = -1;
volatile int8_t n = 0;

ISR(INT0_vect) {
    counterSW1 = EICRA == 0b10 ? DEBOUNCE_ITERATIONS : -1;
    EICRA ^= 0b01;
}

ISR(PCINT2_vect) { counterSW2 = DEBOUNCE_ITERATIONS; }

int main() {
    DDRB = 0b10111;
    DDRD &= ~BUTTONS;
    PORTD |= BUTTONS;

    // Set PD2 interrupt handler
    EIMSK |= 1 << INT0;
    EICRA = 0b10;

    // Set PD4 interrupt handler
    PCICR |= 1 << PCIE2; // Enable pin change interrupts for PCINT23..16
    PCMSK2 |= 1 << PCINT20; // Enable PCINT for PD4

    sei();

    // We increment by 2 and decrement by one because we detect only SW1 down, and all SW2 changes
    while (true) {
        if (counterSW1 >= 0) --counterSW1;
        if (counterSW1 == 0) n += 2;
        if (counterSW2 >= 0) --counterSW2;
        if (counterSW2 == 0) --n;
        PORTB = (n & 14) >> 1 | (n & 16);
        _delay_ms(1);
    }

    return 0;
}
