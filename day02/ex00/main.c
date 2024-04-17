#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define DEBOUNCE_DELAY 20
#define LED (1 << PB0)
#define PIN (1 << PD2)

volatile bool led_is_on = false;

int main() {
    DDRB = LED;
    DDRD &= ~PIN;
    PORTD |= PIN;

    EIMSK |= 1 << INT0;
    EICRA = 0b00;

    sei();

    while (true) {
        PORTB = led_is_on ? LED : 0;
        _delay_ms(DEBOUNCE_DELAY);
        EIMSK |= 1 << INT0;
    }
}

ISR(INT0_vect) {
    if (EICRA == 0b00) led_is_on = !led_is_on;
    EICRA ^= 0b11;
    EIMSK &= ~(1 << INT0);
}
