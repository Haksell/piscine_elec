
#include "main.h"

#define DEBOUNCE_ITERATIONS 100

#define ADDR_COUNTERS ((uint8_t*)0x93)
#define ADDR_SELECTED (ADDR_COUNTERS + 2)
#define ADDR_COUNTER(SELECTED) (ADDR_COUNTERS + 3 + (SELECTED))
#define NUM_COUNTERS 4

static void leds() {
    uint8_t selected = eeprom_read_byte(ADDR_SELECTED);
    uint8_t n = eeprom_read_byte(ADDR_COUNTER(selected));
    PORTB = (n & 7) | (n & 8) << 1;
}

static void print_state() {
    uint8_t buffer[NUM_COUNTERS + 1];
    safe_eeprom_read(buffer, (size_t)ADDR_COUNTERS, sizeof(buffer));
    uint8_t selected = buffer[0];
    if (selected > NUM_COUNTERS) {
        uart_putstr("Out of range selector: ");
        uart_putnbr(selected);
        uart_putstr(". ");
    }
    for (uint8_t i = 0; i < NUM_COUNTERS; ++i) {
        if (i != 0) uart_tx(' ');
        if (i == selected) uart_tx('[');
        uart_putnbr(buffer[i + 1]);
        if (i == selected) uart_tx(']');
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
    bool success = safe_eeprom_write(buffer, (size_t)ADDR_COUNTERS, sizeof(buffer));
    uart_putstrln(success ? "Resetting counters!" : "Loading counters...");
    update_state();
}

int main() {
    uart_init();
    reset_counters();
    DDRB = 0b10111;
    DDRD &= ~(1 << PD2 | 1 << PD4);
    int8_t sw1 = -1;
    int8_t sw2 = -1;
    while (true) {
        if (is_pressed(PD2, &sw1)) {
            uint8_t* addr = ADDR_COUNTER(eeprom_read_byte(ADDR_SELECTED));
            eeprom_write_byte(addr, eeprom_read_byte(addr) + 1);
            update_state();
        }
        if (is_pressed(PD4, &sw2)) {
            uint8_t selected = (eeprom_read_byte(ADDR_SELECTED) + 1) % NUM_COUNTERS;
            eeprom_write_byte(ADDR_SELECTED, selected);
            update_state();
        }
    }
}
