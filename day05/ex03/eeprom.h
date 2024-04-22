#pragma once

#include "main.h"

#define EEPROM_BYTES 64 // Actually 1024. Smaller for testing purposes.
#define EEPROM_MAGIC_BYTES 2
#define EEPROM_MAGIC 0x07CE
#define EEPROM_MAX_ADDR (EEPROM_BYTES - 6)

typedef uint16_t eeprom_size_t;
typedef int16_t eeprom_ssize_t;

bool eepromalloc_init();
bool eepromalloc_write(eeprom_size_t id, void* buffer, eeprom_size_t length);
bool eepromalloc_read(eeprom_size_t id, void* buffer, eeprom_size_t length);
bool eepromalloc_free(uint16_t id);

// TODO: macros to avoid this code repetition
// eeprom_size_t current_id = eeprom_read_word((uint16_t*)addr);
// eeprom_size_t current_capacity = eeprom_read_word((uint16_t*)(addr + 2));
// eeprom_size_t current_length = eeprom_read_word((uint16_t*)(addr + 4));
