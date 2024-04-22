#pragma once

#include "main.h"

bool safe_eeprom_read(void* buffer, size_t offset, size_t length);
bool safe_eeprom_write(void* buffer, size_t offset, size_t length);
