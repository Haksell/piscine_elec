#include "main.h"

#define DELAY_MS 50

void light_show() {
    DDRB = 0b10111;
    DDRD = 0b1101000;
    PORTB = PORTD = 0;
    uint8_t n = 0;
    while (true) {
        ++n;
        _delay_ms(DELAY_MS);
        if (n & 1) PORTB ^= 1 << PB0;
        else if (n & 2) PORTB ^= 1 << PB1;
        else if (n & 4) PORTB ^= 1 << PB2;
        else PORTB ^= 1 << PB4;
        _delay_ms(DELAY_MS);
        if (n & 1) PORTD ^= 1 << 6;
        else if (n & 2) PORTD ^= 1 << 5;
        else PORTD ^= 1 << 3;
    }
}
