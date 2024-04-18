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
        PORTB = (n & 7) | (n & 8) << 1;
        _delay_ms(DELAY_MS);
        uint8_t l3b = n & 7;
        if (n & 1) PORTD ^= 1 << 6;
        else if (n & 2) PORTD ^= 1 << 5;
        else PORTD ^= 1 << 3;
    }
}
