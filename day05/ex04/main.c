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
    size_t addr = EEPROM_MAGIC_BYTES;
    while (addr < EEPROM_MAX_ADDR) {
        size_t current_id = EEPROMALLOC_ID(addr);
        bool is_free = current_id == 0;
        size_t current_capacity = EEPROMALLOC_CAPACITY(addr);
        size_t current_length = EEPROMALLOC_LENGTH(addr);
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

/*
> READ "key-1"
> WRITE "key-1" "dunno"
Done.
> READ "key-1"
"dunno"
> FORGET "key-2"
Not found.
> FORGET "key-1"
Done.
> READ "key-1"
*/

#define COMMAND_BUFFER_SIZE 128

typedef enum {
    COMMAND_INVALID,
    COMMAND_READ,
    COMMAND_WRITE,
    COMMAND_FORGET,
} Command;

static Command
parse_command(char* s, __attribute__((unused)) char* arg1, __attribute__((unused)) char* arg2) {
    while (*s == ' ') ++s;
    size_t i = 0;
    while (s[i] && s[i] != ' ') ++i;
    if (!s[i]) return COMMAND_INVALID;
    s[i] = '\0';
    Command command;
    if (str_equals(s, "READ")) command = COMMAND_READ;
    else if (str_equals(s, "WRITE")) command = COMMAND_WRITE;
    else if (str_equals(s, "FORGET")) command = COMMAND_FORGET;
    else return COMMAND_INVALID;
    s += i + 1;
    while (*s == ' ') ++s;
    uart_putstrln(s);
    return command;
}

int main() {
    uart_init();
    uart_putstrln(eepromalloc_init() ? "Memory initialized." : "Loading memory...");
    memorydump();
    while (true) {
        char buffer[COMMAND_BUFFER_SIZE];
        uart_readline(buffer, COMMAND_BUFFER_SIZE);
        if (str_is_space(buffer)) continue;
        if (str_equals(buffer, ".")) {
            memorydump();
            continue;
        }
        char arg1[COMMAND_BUFFER_SIZE];
        char arg2[COMMAND_BUFFER_SIZE];
        switch (parse_command(buffer, arg1, arg2)) {
            case COMMAND_INVALID: uart_putstrln("COMMAND_INVALID"); break;
            case COMMAND_READ: uart_putstrln("COMMAND_READ"); break;
            case COMMAND_WRITE: uart_putstrln("COMMAND_WRITE"); break;
            case COMMAND_FORGET: uart_putstrln("COMMAND_FORGET"); break;
        }
    }
}
