#include "main.h"

#define EEPROM_MAGIC 0x07CB // TODO: 0x07CE
#define EEPROM_FREED 0xFF

bool eepromalloc_init() {
    if (eeprom_read_word((uint16_t*)0) == EEPROM_MAGIC) return false;
    eeprom_update_word((uint16_t*)0, EEPROM_MAGIC); // magic
    eeprom_update_word((uint16_t*)2, 0); // id
    eeprom_update_word((uint16_t*)4, EEPROM_BYTES - 6 - EEPROM_MAGIC_BYTES); // capacity
    return true;
}

bool eepromalloc_write(eeprom_size_t id, void* buffer, eeprom_size_t length) {
    static eeprom_size_t limit = EEPROM_BYTES - 6;
    eeprom_size_t addr = EEPROM_MAGIC_BYTES;
    while (addr < limit) {
        eeprom_size_t current_id = eeprom_read_word((uint16_t*)addr);
        eeprom_size_t current_capacity = eeprom_read_word((uint16_t*)(addr + 2));
        if (current_id == 0) {
            if (current_capacity >= length) {
                eeprom_size_t remaining_capacity = current_capacity - length;
                bool keep_remaining_capacity = remaining_capacity < 6;
                eeprom_update_word((uint16_t*)addr, id);
                eeprom_update_word(
                    (uint16_t*)(addr + 2),
                    keep_remaining_capacity ? length + remaining_capacity : length
                );
                eeprom_update_word((uint16_t*)(addr + 4), length);
                // eeprom_update_block?
                for (eeprom_size_t i = 0; i < length; ++i)
                    eeprom_update_byte((uint8_t*)(addr + i + 6), ((uint8_t*)buffer)[i]);
                if (!keep_remaining_capacity) {
                    eeprom_size_t next_addr = addr + length + 6;
                    eeprom_update_word((uint16_t*)next_addr, 0);
                    eeprom_update_word((uint16_t*)(next_addr + 2), remaining_capacity - 6);
                }
                return true;
            }
        } else if (current_id == id) {
            if (current_capacity >= length) {
                eeprom_update_word((uint16_t*)(addr + 4), length);
                for (eeprom_size_t i = 0; i < length; ++i)
                    eeprom_update_byte((uint8_t*)(addr + i + 6), ((uint8_t*)buffer)[i]);
                return true;
            } else {
                // TODO: merge with previous and next blocks (same as eepromalloc_free)
                // TODO: then go find somewhere else
                return false;
            }
        }
        addr += 6 + current_capacity;
    }
    return false;
}

// bool eepromalloc_read(eeprom_size_t id, void* buffer, eeprom_size_t length) {

// }

// bool eepromalloc_free(eeprom_size_t id);
