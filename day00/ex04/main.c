#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define DEBOUNCE_ITERATIONS 5

int main() {
    DDRB = 0b10111; // Configure pins PB0, Pb1, PB2, PB4 as output
    int n = 0; // Value to display
    int counterSW1 = 0;
    int counterSW2 = 0;
    while (true) {
        counterSW1 = (PIND >> PD2) & 1 ? 0 : counterSW1 + 1;
        counterSW2 = (PIND >> PD4) & 1 ? 0 : counterSW2 + 1;
        if (counterSW1 == DEBOUNCE_ITERATIONS) ++n;
        if (counterSW2 == DEBOUNCE_ITERATIONS) --n;
        int portb = 0;
        if (n & 1) portb |= 1 << PB0;
        if (n & 2) portb |= 1 << PB1;
        if (n & 4) portb |= 1 << PB2;
        if (n & 8) portb |= 1 << PB4;
        PORTB = portb;
        _delay_ms(10);
    }
    return 0;
}
