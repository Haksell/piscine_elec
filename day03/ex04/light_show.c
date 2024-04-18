#include "main.h"

void light_show() {
    DDRB = 0b10111;
    DDRD = 0b1101000;
    uint8_t n = 0;
    while (true) {
        PORTB = (n & 7) | (n & 8) << 1;
        PORTD = (n & 3) << 5 | (n & 4) << 1;
        ++n;
        _delay_ms(100);
    }
}
