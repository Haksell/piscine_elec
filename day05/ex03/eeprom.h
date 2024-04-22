#pragma once

#include "main.h"

#if DEBUG
#define EEPROM_BYTES 64
#else
#define EEPROM_BYTES 1024
#endif

#define EEPROM_MAGIC_BYTES 2
#define EEPROM_MAGIC 0xce07
#define EEPROM_MAX_ADDR (EEPROM_BYTES - 6)

typedef uint16_t eeprom_size_t;
typedef int16_t eeprom_ssize_t;

#define EEPROMALLOC_ID(ADDR) (eeprom_read_word((uint16_t*)(ADDR)))
#define EEPROMALLOC_CAPACITY(ADDR) (eeprom_read_word((uint16_t*)((ADDR) + 2)))
#define EEPROMALLOC_LENGTH(ADDR) (eeprom_read_word((uint16_t*)((ADDR) + 4)))

bool eepromalloc_init();
bool eepromalloc_write(eeprom_size_t id, void* buffer, eeprom_size_t length);
bool eepromalloc_read(eeprom_size_t id, void* buffer, eeprom_size_t length);
bool eepromalloc_free(uint16_t id);
