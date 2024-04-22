#pragma once

#include "main.h"

bool safe_eeprom_read(void* buffer, size_t offset, size_t length);
bool safe_eeprom_write(void* buffer, size_t offset, size_t length);

bool safe_eeprom_read_byte(size_t offset, size_t idx, uint8_t* byte);
bool safe_eeprom_write_byte(size_t offset, size_t idx, uint8_t byte);
