#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define BUTTONS (1 << PD2 | 1 << PD4)
#define DEBOUNCE_ITERATIONS 7

volatile int8_t counter_sw1 = -1;
volatile int8_t counter_sw2 = -1;
volatile bool sw1_is_pressed = false;
volatile bool sw2_is_pressed = false;
volatile int8_t n = 0;

ISR(INT0_vect) { counter_sw1 = DEBOUNCE_ITERATIONS; }
ISR(PCINT2_vect) { counter_sw2 = DEBOUNCE_ITERATIONS; }

int main() {
    DDRB = 0b10111;
    DDRD &= ~BUTTONS;
    PORTD |= BUTTONS;

    // Set PD2 interrupt handler
    EIMSK |= 1 << INT0;
    EICRA = 0b01;

    // Set PD4 interrupt handler
    PCICR |= 1 << PCIE2; // Enable pin change interrupts for PCINT23..16
    PCMSK2 |= 1 << PCINT20; // Enable PCINT for PD4

    sei();
    while (true) {
        if (counter_sw1 >= 0) --counter_sw1;
        if (counter_sw1 == 0) n += sw1_is_pressed = !sw1_is_pressed;
        if (counter_sw2 >= 0) --counter_sw2;
        if (counter_sw2 == 0) n -= sw2_is_pressed = !sw2_is_pressed;
        PORTB = n & 7 | (n & 8) << 1;
        _delay_ms(1);
    }
}
