#include "main.h"

bool eepromalloc_init() {
    if (eeprom_read_word((uint16_t*)0) == EEPROM_MAGIC) return false;
    eeprom_update_word((uint16_t*)0, EEPROM_MAGIC); // magic
    eeprom_update_word((uint16_t*)2, 0); // id
    eeprom_update_word((uint16_t*)4, EEPROM_BYTES - 6 - EEPROM_MAGIC_BYTES); // capacity
    return true;
}

bool eepromalloc_write(eeprom_size_t id, void* buffer, eeprom_size_t length) {
    if (id == 0) return false;
    eeprom_size_t last_free_block = 0;
    eeprom_size_t addr = EEPROM_MAGIC_BYTES;
    while (addr < EEPROM_MAX_ADDR) {
        eeprom_size_t current_id = EEPROMALLOC_ID(addr);
        eeprom_size_t current_capacity = EEPROMALLOC_CAPACITY(addr);
        if (current_id == 0 && current_capacity >= length) last_free_block = addr;
        else if (current_id == id) {
            if (current_capacity >= length) {
                eeprom_update_word((uint16_t*)(addr + 4), length);
                for (eeprom_size_t i = 0; i < length; ++i)
                    eeprom_update_byte((uint8_t*)(addr + i + 6), ((uint8_t*)buffer)[i]);
                return true;
            } else {
                eepromalloc_free(id); // NOTE: frees even if realloc fails
                return eepromalloc_write(id, buffer, length);
            }
        }
        addr += 6 + current_capacity;
    }
    if (last_free_block == 0) return false; // TODO: reclaim memory from capacity > length
    eeprom_size_t current_capacity = EEPROMALLOC_CAPACITY(last_free_block);
    eeprom_size_t remaining_capacity = current_capacity - length;
    bool keep_remaining_capacity = remaining_capacity < 6;
    eeprom_update_word((uint16_t*)last_free_block, id);
    eeprom_update_word(
        (uint16_t*)(last_free_block + 2),
        keep_remaining_capacity ? length + remaining_capacity : length
    );
    eeprom_update_word((uint16_t*)(last_free_block + 4), length);
    eeprom_update_block(buffer, (void*)(last_free_block + 6), length);
    if (!keep_remaining_capacity) {
        eeprom_size_t next_addr = last_free_block + length + 6;
        eeprom_update_word((uint16_t*)next_addr, 0);
        eeprom_update_word((uint16_t*)(next_addr + 2), remaining_capacity - 6);
    }
    return true;
}

bool eepromalloc_read(eeprom_size_t id, void* buffer, eeprom_size_t length) {
    eeprom_size_t addr = EEPROM_MAGIC_BYTES;
    while (addr < EEPROM_MAX_ADDR) {
        eeprom_size_t current_id = EEPROMALLOC_ID(addr);
        eeprom_size_t current_capacity = EEPROMALLOC_CAPACITY(addr);
        eeprom_size_t current_length = EEPROMALLOC_LENGTH(addr);
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

bool eepromalloc_free(eeprom_size_t id) {
    eeprom_size_t prev_addr;
    bool prev_is_free = false;
    eeprom_size_t addr = EEPROM_MAGIC_BYTES;
    while (addr < EEPROM_MAX_ADDR) {
        eeprom_size_t current_id = EEPROMALLOC_ID(addr);
        eeprom_size_t current_capacity = EEPROMALLOC_CAPACITY(addr);
        if (current_id == id) {
            eeprom_size_t next_addr = addr + 6 + current_capacity;
            bool next_is_free = next_addr < EEPROM_MAX_ADDR && EEPROMALLOC_ID(next_addr) == 0;
            if (prev_is_free) {
                addr = prev_addr;
                current_capacity += EEPROMALLOC_CAPACITY(addr) + 6;
            } else eeprom_update_word((uint16_t*)addr, 0);
            if (next_is_free) current_capacity += EEPROMALLOC_CAPACITY(next_addr) + 6;
            eeprom_update_word((uint16_t*)(addr + 2), current_capacity);
            return true;
        }
        prev_is_free = current_id == 0;
        prev_addr = addr;
        addr += 6 + current_capacity;
    }
    return false;
}
