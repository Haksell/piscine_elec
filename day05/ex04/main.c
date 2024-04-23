#include "main.h"

#define BUFFER_SIZE 128

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
            for (size_t i = 0; i < current_length; ++i)
                uart_tx(eeprom_read_byte((uint8_t*)(addr + 6 + i)));
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

typedef enum {
    COMMAND_INVALID,
    COMMAND_WRITE,
    COMMAND_READ,
    COMMAND_FORGET,
} Command;

static bool parse_arg(char** buf_ptr, char* arg) {
    if (**buf_ptr != '"') return false;
    char* buf = *buf_ptr + 1;
    size_t i = 0;
    while (*buf != '"') {
        if (*buf == '\\') ++buf;
        if (*buf == '\0') return false;
        arg[i] = *buf;
        ++i;
        ++buf;
    }
    arg[i] = '\0';
    *buf_ptr = buf + 1;
    return true;
}

static Command parse_command(char* s, char* key, __attribute__((unused)) char* value) {
    while (*s == ' ') ++s;
    size_t i = 0;
    while (s[i] && s[i] != ' ') ++i;
    if (!s[i]) return COMMAND_INVALID;
    s[i] = '\0';
    Command command;
    if (str_equals(s, "WRITE")) command = COMMAND_WRITE;
    else if (str_equals(s, "READ")) command = COMMAND_READ;
    else if (str_equals(s, "FORGET")) command = COMMAND_FORGET;
    else return COMMAND_INVALID;
    s += i + 1;
    while (*s == ' ') ++s;
    if (!parse_arg(&s, key)) return COMMAND_INVALID;
    if (command == COMMAND_WRITE) {
        if (*s != ' ') return COMMAND_INVALID;
        while (*s == ' ') ++s;
        if (!parse_arg(&s, value)) return COMMAND_INVALID;
    }
    while (*s == ' ') ++s;
    return *s ? COMMAND_INVALID : command;
}

static void dict_write(char* key, char* value) {
    char key_value[BUFFER_SIZE], buffer[BUFFER_SIZE];
    const size_t len_key = ft_strlen(key);
    const size_t len_value = ft_strlen(value);
    ft_strcpy(key_value, key);
    key_value[len_key] = '\0';
    ft_strcpy(key_value + len_key + 1, value);

    size_t id = hash((uint8_t*)key);
    while (true) {
        ssize_t read_bytes = eepromalloc_read(id, buffer, BUFFER_SIZE);
        if (read_bytes > 0) ++id;
        else {
            if (read_bytes == 0) eepromalloc_free(id);
            bool success = eepromalloc_write(id, key_value, len_key + 1 + len_value);
            uart_putstrln(success ? "Done." : "No space left.");
            return;
        }
    }
}

static void dict_read(char* key) {
    char key_value[BUFFER_SIZE];
    size_t id = hash((uint8_t*)key);
    while (true) {
        ssize_t read_bytes = eepromalloc_read(id, key_value, BUFFER_SIZE);
        if (read_bytes == -1) {
            uart_putstrln("Not found.");
            return;
        } else if (read_bytes > 0 && str_equals(key, key_value)) {
            size_t i = 0;
            while (key_value[i]) ++i;
            uart_tx('[');
            uart_putstr(key_value + i + 1);
            uart_putstrln("]");
            return;
        } else ++id;
    }
}

static void dict_forget(char* key) {
    char key_value[BUFFER_SIZE];
    size_t true_id = hash((uint8_t*)key), id = true_id, start_free_id = true_id;
    while (true) {
        ssize_t read_bytes = eepromalloc_read(id, key_value, BUFFER_SIZE);
        if (read_bytes == -1) {
            uart_putstrln("Not found.");
            return;
        } else if (read_bytes > 0 && str_equals(key, key_value)) {
            if (eepromalloc_read(id + 1, key_value, BUFFER_SIZE) >= 0)
                eepromalloc_write(id, NULL, 0);
            else {
                while (id != start_free_id) {
                    eepromalloc_free(id);
                    --id;
                }
                eepromalloc_free(id);
            }
            uart_putstrln("Done.");
            return;
        } else {
            ++id;
            if (read_bytes > 0) start_free_id = id;
        }
    }
}

int main() {
    uart_init();
    uart_putstrln(eepromalloc_init() ? "Memory initialized." : "Loading memory...");
    memorydump();
    while (true) {
        char buffer[BUFFER_SIZE];
        uart_readline(buffer, BUFFER_SIZE);
        if (str_is_space(buffer)) continue;
        if (str_equals(buffer, ".")) {
            memorydump();
            continue;
        }
        char key[BUFFER_SIZE];
        char value[BUFFER_SIZE];
        switch (parse_command(buffer, key, value)) {
            case COMMAND_INVALID: uart_putstrln("Invalid command."); break;
            case COMMAND_WRITE: dict_write(key, value); break;
            case COMMAND_READ: dict_read(key); break;
            case COMMAND_FORGET: dict_forget(key); break;
        }
    }
}
