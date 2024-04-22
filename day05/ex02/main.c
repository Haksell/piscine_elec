
#include "main.h"

#define DEBOUNCE_ITERATIONS 100

#define COUNTERS_ADDR ((uint8_t*)0x67)
#define COUNTER_ADDR(SELECTED) (COUNTERS_ADDR + 1 + SELECTED)
#define NUM_COUNTERS 4

static void leds() {
    uint8_t selected = eeprom_read_byte(COUNTERS_ADDR);
    uint8_t n = eeprom_read_byte(COUNTER_ADDR(selected));
    PORTB = (n & 7) | (n & 8) << 1;
}

static void print_state() {
    uint8_t buffer[NUM_COUNTERS + 1];
    bool success = safe_eeprom_read(buffer, (size_t)COUNTERS_ADDR, sizeof(buffer));
    uart_putstr(success ? "success " : "fail ");
    // uint8_t selected = buffer[0];
    for (uint8_t i = 0; i < NUM_COUNTERS; ++i) {
        if (i != 0) uart_tx(' ');
        uart_putnbr(buffer[i + 1]);
    }
    uart_putstrln("");
}

static void update_state() {
    leds();
    print_state();
}

static bool is_pressed(uint8_t pin, int8_t* debounce) {
    *debounce = (PIND >> pin) & 1 ? DEBOUNCE_ITERATIONS : *debounce - (*debounce >= 0);
    return *debounce == 0;
}

static void reset_counters() {
    uint8_t buffer[NUM_COUNTERS + 1] = {0};
    safe_eeprom_write(buffer, (size_t)COUNTERS_ADDR, sizeof(buffer));
}

int main() {
    uart_init();
    DDRB = 0b10111;
    DDRD &= ~(1 << PD2 | 1 << PD4);
    reset_counters();
    update_state();
    int8_t sw1 = -1;
    int8_t sw2 = -1;
    while (true) {
        if (is_pressed(PD2, &sw1)) {
            uint8_t* addr = COUNTER_ADDR(eeprom_read_byte(COUNTERS_ADDR));
            eeprom_write_byte(addr, eeprom_read_byte(addr) + 1);
            update_state();
        }
        if (is_pressed(PD4, &sw2)) {
            uint8_t selected = (eeprom_read_byte(COUNTERS_ADDR) + 1) % NUM_COUNTERS;
            eeprom_write_byte(COUNTERS_ADDR, selected);
            update_state();
        }
    }
}
