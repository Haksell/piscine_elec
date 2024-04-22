#include "main.h"

bool eepromalloc_init() {
    if (eeprom_read_word((uint16_t*)0) == EEPROM_MAGIC) return false;
    eeprom_update_word((uint16_t*)0, EEPROM_MAGIC);
    eeprom_update_word((uint16_t*)2, 0);
    eeprom_update_word((uint16_t*)4, EEPROM_TOTAL_CAPACITY);
    return true;
}

bool eepromalloc_write(size_t id, void* buffer, size_t length) {
    if (id == 0 || length > EEPROM_TOTAL_CAPACITY) return false;
    size_t free_block_addr = 0;
    size_t free_block_capacity = EEPROM_BYTES;
    size_t addr = EEPROM_MAGIC_BYTES;
    while (addr < EEPROM_MAX_ADDR) {
        size_t current_id = EEPROMALLOC_ID(addr);
        size_t current_capacity = EEPROMALLOC_CAPACITY(addr);
        if (current_id == 0 && current_capacity >= length &&
            current_capacity < free_block_capacity) {
            free_block_capacity = current_capacity;
            free_block_addr = addr;
        } else if (current_id == id) {
            if (current_capacity >= length) {
                eeprom_update_word((uint16_t*)(addr + 4), length);
                for (size_t i = 0; i < length; ++i)
                    eeprom_update_byte((uint8_t*)(addr + i + 6), ((uint8_t*)buffer)[i]);
                if (length + 6 <= current_capacity) {
                    eeprom_update_word((uint16_t*)(addr + 2), length);
                    size_t free_addr = addr + 6 + length;
                    eeprom_update_word((uint16_t*)free_addr, 0);
                    size_t free_capacity = current_capacity - 6 - length;
                    size_t next_addr = free_addr + 6 + free_capacity;
                    bool next_is_free = next_addr < EEPROM_MAX_ADDR &&
                                        EEPROMALLOC_ID(next_addr) == 0;
                    if (next_is_free) free_capacity += EEPROMALLOC_CAPACITY(next_addr) + 6;
                    eeprom_update_word((uint16_t*)(addr + 8 + length), free_capacity);
                }
                return true;
            } else {
                eepromalloc_free(id); // NOTE: frees even if realloc fails
                return eepromalloc_write(id, buffer, length);
            }
        }
        addr += 6 + current_capacity;
    }
    if (free_block_addr == 0) return false; // TODO: reclaim memory from capacity > length
    size_t current_capacity = EEPROMALLOC_CAPACITY(free_block_addr);
    size_t remaining_capacity = current_capacity - length;
    bool keep_remaining_capacity = remaining_capacity < 6;
    eeprom_update_word((uint16_t*)free_block_addr, id);
    eeprom_update_word(
        (uint16_t*)(free_block_addr + 2),
        keep_remaining_capacity ? length + remaining_capacity : length
    );
    eeprom_update_word((uint16_t*)(free_block_addr + 4), length);
    eeprom_update_block(buffer, (void*)(free_block_addr + 6), length);
    if (!keep_remaining_capacity) {
        size_t next_addr = free_block_addr + length + 6;
        eeprom_update_word((uint16_t*)next_addr, 0);
        eeprom_update_word((uint16_t*)(next_addr + 2), remaining_capacity - 6);
    }
    return true;
}

bool eepromalloc_read(size_t id, void* buffer, size_t length) {
    size_t addr = EEPROM_MAGIC_BYTES;
    while (addr < EEPROM_MAX_ADDR) {
        size_t current_id = EEPROMALLOC_ID(addr);
        size_t current_capacity = EEPROMALLOC_CAPACITY(addr);
        size_t current_length = EEPROMALLOC_LENGTH(addr);
        if (current_id == id) {
            if (current_length < length) {
                eeprom_read_block(buffer, (void*)(addr + 6), current_length);
                ((char*)buffer)[current_length] = '\0';
                return true;
            } else return false;
        }
        addr += 6 + current_capacity;
    }
    return false;
}

bool eepromalloc_free(size_t id) {
    size_t prev_addr;
    bool prev_is_free = false;
    size_t addr = EEPROM_MAGIC_BYTES;
    while (addr < EEPROM_MAX_ADDR) {
        size_t current_id = EEPROMALLOC_ID(addr);
        size_t current_capacity = EEPROMALLOC_CAPACITY(addr);
        if (current_id == id) {
            size_t next_addr = addr + 6 + current_capacity;
            bool next_is_free = next_addr < EEPROM_MAX_ADDR && EEPROMALLOC_ID(next_addr) == 0;
            if (next_is_free) current_capacity += EEPROMALLOC_CAPACITY(next_addr) + 6;
            if (prev_is_free) {
                addr = prev_addr;
                current_capacity += EEPROMALLOC_CAPACITY(addr) + 6;
            } else eeprom_update_word((uint16_t*)addr, 0);
            eeprom_update_word((uint16_t*)(addr + 2), current_capacity);
            return true;
        }
        prev_is_free = current_id == 0;
        prev_addr = addr;
        addr += 6 + current_capacity;
    }
    return false;
}
