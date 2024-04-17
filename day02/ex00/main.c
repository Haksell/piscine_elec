#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

volatile bool button_is_pressed = false;
volatile bool led_is_on = false;

int main() {
    DDRB = 1 << PB0;
    EIMSK = 1 << INT0;
    EICRA = 0b00;

    sei();

    while (true) {
        EIMSK |= 1 << INT0;
        PORTB = led_is_on ? 1 << PB0 : 0;
        _delay_ms(20);
    }
}

ISR(INT0_vect) {
    if (EICRA == 0b00) {
        button_is_pressed = true;
        led_is_on = !led_is_on;
        EICRA = 0b11;
    } else {
        button_is_pressed = false;
        EICRA = 0b00;
    }
    EIMSK &= ~(1 << INT0);
}
