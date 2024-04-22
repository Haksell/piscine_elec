#include "main.h"

#define DEBOUNCE_ITERATIONS 7
#define COUNTER_ADDR ((uint8_t*)0x42)

// #include <util/delay.h> // TODO: remove

int main() {
    // uart_init(); // TODO: remove
    DDRB = 0b10111;
    eeprom_write_byte(COUNTER_ADDR, 0);
    uint8_t counterSW1 = 0;
    uint8_t counterSW2 = 0;
    while (true) {
        counterSW1 = (PIND >> PD2) & 1 ? 0 : counterSW1 + (counterSW1 <= DEBOUNCE_ITERATIONS);
        counterSW2 = (PIND >> PD4) & 1 ? 0 : counterSW2 + (counterSW2 <= DEBOUNCE_ITERATIONS);
        uint8_t n = eeprom_read_byte(COUNTER_ADDR);
        if (counterSW1 == DEBOUNCE_ITERATIONS) ++n;
        if (counterSW2 == DEBOUNCE_ITERATIONS) --n;
        PORTB = (n & 7) | (n & 8) << 1;
        // uart_putnbrln(n);
        eeprom_write_byte(COUNTER_ADDR, n);
    }
}
