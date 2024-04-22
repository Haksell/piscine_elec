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
        eeprom_size_t current_id = EEPROMALLOC_ID(addr);
        bool is_free = current_id == 0;
        eeprom_size_t current_capacity = EEPROMALLOC_CAPACITY(addr);
        eeprom_size_t current_length = EEPROMALLOC_LENGTH(addr);
        uart_tx('[');
        if (is_free) {
            uart_putnbr(current_capacity);
            uart_putstr("...");
        } else {
            uart_putnbr(current_id);
            uart_putstr(": ");
            uart_putnbr(current_length);
            uart_tx('/');
            uart_putnbr(current_capacity);
        }
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
    static const size_t limit_init = SIZE_MAX / 10;
    static const size_t limit_last = SIZE_MAX % 10;
    for (*id = 0, *i = 0; ft_isdigit(str[*i]); ++*i) {
        size_t digit = str[*i] - '0';
        if (*id > limit_init || (*id == limit_init && digit > limit_last)) return false;
        *id = 10 * *id + digit;
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
        if (buffer[0] == '.' && buffer[1] == '\0') {
            memorydump();
            continue;
        }
        bool is_read = buffer[0] == '?';
        bool is_free = buffer[0] == '!';
        size_t id, i;
        if (!atoi_id(buffer + (is_read || is_free), &id, &i)) {
            uart_putstrln("Invalid id.");
            continue;
        }
        if (is_read) {
            if (eepromalloc_read(id, buffer, EEPROM_BYTES)) {
                uart_tx('[');
                uart_putstr(buffer);
                uart_putstrln("]");
            } else uart_putstrln("eepromalloc_read: id not found");
        } else if (is_free) {
            if (eepromalloc_free(id)) memorydump();
            else uart_putstrln("eepromalloc_free: id not found");
        } else {
            char* value = buffer + i;
            if (eepromalloc_write(id, value, ft_strlen(value))) memorydump();
            else uart_putstrln("eepromalloc_write: somehow failed");
        }
    }
}
