
#include "main.h"

#include <util/delay.h> // TODO

void hexdump() {
    for (size_t i = 0; i < EEPROM_BYTES; ++i) {
        uint8_t spaces = i == 0 ? 0 : (i & 7) == 0 ? 2 : 1;
        for (uint8_t j = 0; j < spaces; ++j) uart_tx(' ');
        uart_putbyte(eeprom_read_byte((uint8_t*)i));
    }
    uart_newline();
}

int main() {
    uart_init();
    uart_putstrln(eepromalloc_init() ? "Initialized." : "Loading memory...");
    while (true) {
        char buf[] = "mdr";
        eepromalloc_write(1, buf, sizeof(buf));
        hexdump();
        break;
    }
}
