#include <avr/io.h>
#include <stdbool.h>

#define LED (1 << PB1)

int main() {
    const uint32_t LOOP_CYCLES = 14;
    const uint32_t RESET_COUNTER = F_CPU / LOOP_CYCLES / 2 - 1;
    DDRB = LED;
    uint32_t i = 0;
    while (true) {
        if (i == RESET_COUNTER) {
            PORTB ^= LED;
            i = 0;
        } else ++i;
    }
}
