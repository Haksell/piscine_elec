#include <avr/eeprom.h>
#include <avr/io.h>
#include <stdbool.h>

#define DEBOUNCE_ITERATIONS 100
#define COUNTER_ADDR ((uint8_t*)0x42)

int main() {
    DDRB |= 0b10111;
    DDRD &= ~(1 << PD2);
    eeprom_write_byte(COUNTER_ADDR, 0);
    int8_t counterSW1 = -1;
    while (true) {
        counterSW1 = (PIND >> PD2) & 1 ? DEBOUNCE_ITERATIONS : counterSW1 - (counterSW1 >= 0);
        if (counterSW1 == 0) {
            uint8_t n = eeprom_read_byte(COUNTER_ADDR) + 1;
            PORTB = (n & 7) | (n & 8) << 1;
            eeprom_write_byte(COUNTER_ADDR, n);
        }
    }
}
