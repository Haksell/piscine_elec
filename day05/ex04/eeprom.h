#pragma once

#include "main.h"

#if DEBUG
#define EEPROM_BYTES 64
#else
#define EEPROM_BYTES 1024
#endif

typedef int16_t ssize_t;

#define EEPROM_MAGIC_BYTES 2
#define EEPROM_MAGIC (0x92ff ^ DEBUG)
#define EEPROM_MAX_ADDR (EEPROM_BYTES - 6)
#define EEPROM_TOTAL_CAPACITY (EEPROM_BYTES - 6 - EEPROM_MAGIC_BYTES)

#define EEPROMALLOC_ID(ADDR) (eeprom_read_word((uint16_t*)(ADDR)))
#define EEPROMALLOC_CAPACITY(ADDR) (eeprom_read_word((uint16_t*)((ADDR) + 2)))
#define EEPROMALLOC_LENGTH(ADDR) (eeprom_read_word((uint16_t*)((ADDR) + 4)))

bool eepromalloc_init();
bool eepromalloc_write(size_t id, void* buffer, size_t length);
ssize_t eepromalloc_read(size_t id, void* buffer, size_t length);
bool eepromalloc_free(size_t id);
