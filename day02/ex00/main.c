#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

volatile bool led_is_on = false;

int main() {
    DDRB = 1 << PB0;
    EIMSK = 1 << INT0;
    EICRA = 0b10;

    sei();

    while (true) {
        PORTB = led_is_on ? 1 << PB0 : 0;
        _delay_ms(10);
    }
}

ISR(INT0_vect) {
    if (EICRA == 0b10) led_is_on = !led_is_on;
    EICRA ^= 0b11;
}
