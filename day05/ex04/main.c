#include "main.h"

#define MAGIC_NUMBER 0x1337
#define EEPROM_BYTES 64 // TODO: 1024

static void hexdump() {
    for (size_t i = 0; i < EEPROM_BYTES; ++i) {
        uint8_t spaces = i == 0 ? 0 : (i & 7) == 0 ? 2 : 1;
        for (uint8_t j = 0; j < spaces; ++j) uart_tx(' ');
        uart_putbyte(eeprom_read_byte((uint8_t*)i));
    }
    uart_newline();
}

static bool eepromalloc_init() {
    if (eeprom_read_word((uint16_t*)0) == MAGIC_NUMBER) return false;
    eeprom_update_word(0, MAGIC_NUMBER);
    eeprom_update_byte((uint8_t*)2, 0);
    return true;
}

int main() {
    uart_init();
    uart_putstrln(eepromalloc_init() ? "Dictionary initialized." : "Loading dictionary...");
    hexdump();
    while (true) { uart_tx(uart_rx()); }
}
