#pragma once

#include "main.h"

#define EEPROM_BYTES 64 // TODO: 1024 actually
#define EEPROM_MAGIC_BYTES 2

typedef uint16_t eeprom_size_t;
typedef int16_t eeprom_ssize_t;

bool eepromalloc_init();
bool eepromalloc_write(eeprom_size_t id, void* buffer, eeprom_size_t length);
// bool eepromalloc_read(uint16_t id, void* buffer, uint16_t length);
// bool eepromalloc_free(uint16_t id);
