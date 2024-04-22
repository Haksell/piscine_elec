#include <avr/io.h>
#include <stdbool.h>

#define DEBOUNCE_ITERATIONS 42

int main() {
    DDRB = 0b10111;
    uint8_t n = 0;
    uint8_t counterSW1 = 0;
    uint8_t counterSW2 = 0;
    while (true) {
        counterSW1 = (PIND >> PD2) & 1 ? 0 : counterSW1 + (counterSW1 <= DEBOUNCE_ITERATIONS);
        counterSW2 = (PIND >> PD4) & 1 ? 0 : counterSW2 + (counterSW2 <= DEBOUNCE_ITERATIONS);
        if (counterSW1 == DEBOUNCE_ITERATIONS) ++n;
        if (counterSW2 == DEBOUNCE_ITERATIONS) --n;
        PORTB = (n & 7) | (n & 8) << 1;
    }
}
