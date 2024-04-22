
#include "main.h"

#define DEBOUNCE_ITERATIONS 100

#define COUNTERS_ADDR ((uint8_t*)0x42)
#define COUNTER_ADDR(SELECTED) (COUNTERS_ADDR + 1 + SELECTED)
#define NUM_COUNTERS 4

static void leds(uint8_t n) {
    PORTB = (n & 7) | (n & 8) << 1;
    uart_putnbrln(42);
}

static bool is_pressed(uint8_t pin, int8_t* debounce) {
    *debounce = (PIND >> pin) & 1 ? DEBOUNCE_ITERATIONS : *debounce - (*debounce >= 0);
    return *debounce == 0;
}

int main() {
    uart_init();
    DDRB = 0b10111;
    DDRD &= ~(1 << PD2 | 1 << PD4);
    for (int i = 0; i <= NUM_COUNTERS; ++i) eeprom_write_byte(COUNTERS_ADDR + i, 0);
    int8_t sw1 = -1;
    int8_t sw2 = -1;
    while (true) {
        if (is_pressed(PD2, &sw1)) {
            uint8_t* addr = COUNTER_ADDR(eeprom_read_byte(COUNTERS_ADDR));
            uint8_t n = eeprom_read_byte(addr) + 1;
            eeprom_write_byte(addr, n);
            leds(n);
        }
        if (is_pressed(PD4, &sw2)) {
            uint8_t selected = (eeprom_read_byte(COUNTERS_ADDR) + 1) % NUM_COUNTERS;
            eeprom_write_byte(COUNTERS_ADDR, selected);
            leds(eeprom_read_byte(COUNTER_ADDR(selected)));
        }
    }
}
