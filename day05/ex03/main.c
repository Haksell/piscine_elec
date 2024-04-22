#include "main.h"

static void hexdump() {
    for (size_t i = 0; i < EEPROM_BYTES; ++i) {
        uint8_t spaces = i == 0 ? 0 : (i & 7) == 0 ? 2 : 1;
        for (uint8_t j = 0; j < spaces; ++j) uart_tx(' ');
        uart_putbyte(eeprom_read_byte((uint8_t*)i));
    }
    uart_newline();
}

static void blockdump() {
    eeprom_size_t addr = EEPROM_MAGIC_BYTES;
    while (addr < EEPROM_MAX_ADDR) {
        eeprom_size_t current_id = CURRENT_ID(addr);
        eeprom_size_t current_capacity = CURRENT_CAPACITY(addr);
        eeprom_size_t current_length = CURRENT_LENGTH(addr);
        uart_tx('[');
        uart_putnbr(current_id);
        uart_putstr(": ");
        if (current_id != 0) {
            uart_putnbr(current_length);
            uart_tx('/');
        }
        uart_putnbr(current_capacity);
        uart_putstr("] ");
        addr += 6 + current_capacity;
    }
    uart_newline();
}

static void memorydump() {
    if (DEBUG) hexdump();
    blockdump();
}

static void uart_readline(char* buffer, size_t buffer_size) {
    size_t i = 0;
    uart_putstr("> ");
    while (true) {
        char c = uart_rx();
        if (c == BACKSPACE) {
            if (i > 0) {
                --i;
                uart_putstr("\b \b");
            }
        } else if (c == '\r') {
            buffer[i] = '\0';
            uart_newline();
            return;
        } else if (ft_isprint(c) && i < buffer_size - 1) {
            buffer[i] = c;
            ++i;
            uart_tx(c);
        }
    }
}

static bool atoi_id(char* str, size_t* id, size_t* i) {
    *id = 0;
    for (*i = 0; ft_isdigit(str[*i]); ++*i) {
        *id = 10 * *id + str[*i] - '0';
        if (*id > UINT16_MAX) return false;
    }
    if (*id == 0) return false;
    if (str[*i] == '\0') return true;
    if (str[*i] == ' ') {
        ++*i;
        return true;
    }
    return false;
}

int main() {
    uart_init();
    uart_putstrln(eepromalloc_init() ? "Memory initialized." : "Loading memory...");
    memorydump();
    while (true) {
        char buffer[EEPROM_BYTES];
        uart_readline(buffer, EEPROM_BYTES);
        if (buffer[0] == '\0') continue;
        bool is_read = buffer[0] == '?';
        bool is_free = buffer[0] == '!';
        size_t id, i;
        if (!atoi_id(buffer + (is_read || is_free), &id, &i)) {
            uart_putstrln("Invalid id.");
            continue;
        }
        if (is_read) {
            bool success = eepromalloc_read(id, buffer, EEPROM_BYTES);
            if (success) {
                uart_tx('[');
                uart_putstr(buffer);
                uart_putstrln("]");
            } else uart_putstrln("eepromalloc_read: id not found");
        } else if (is_free) {
            if (eepromalloc_free(id)) memorydump();
            else uart_putstrln("eepromalloc_write: id not found");
        } else {
            char* value = buffer + i;
            if (eepromalloc_write(id, value, ft_strlen(value))) memorydump();
            else uart_putstrln("eepromalloc_write: somehow failed");
        }
    }
}
