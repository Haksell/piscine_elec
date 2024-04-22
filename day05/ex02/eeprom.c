#include "main.h"

#define EEPROM_BYTES 1024

#define EEPROM_MAGIC_BYTES 2
#define EEPROM_MAGIC_0 0x07
#define EEPROM_MAGIC_1 0xCE

#define EEPROM_ADDR(OFFSET, I) ((uint8_t*)((OFFSET) + (I) + EEPROM_MAGIC_BYTES))

static bool check_args(size_t offset, size_t length) {
    return EEPROM_MAGIC_BYTES + offset + length <= EEPROM_BYTES;
}

static bool check_magic_number(size_t offset) {
    return eeprom_read_byte((uint8_t*)offset) == EEPROM_MAGIC_0 &&
           eeprom_read_byte((uint8_t*)(offset + 1)) == EEPROM_MAGIC_1;
}

bool safe_eeprom_read(void* buffer, size_t offset, size_t length) {
    if (!check_args(offset, length) || !check_magic_number(offset)) return false;
    uint8_t* ptr = buffer;
    for (size_t i = 0; i < length; ++i) ptr[i] = eeprom_read_byte(EEPROM_ADDR(offset, i));
    return true;
}

bool safe_eeprom_write(void* buffer, size_t offset, size_t length) {
    if (!check_args(offset, length) || check_magic_number(offset)) return false;
    eeprom_write_byte((uint8_t*)offset, EEPROM_MAGIC_0);
    eeprom_write_byte((uint8_t*)(offset + 1), EEPROM_MAGIC_1);
    for (size_t i = 0; i < length; ++i) {
        uint8_t* addr = EEPROM_ADDR(offset, i);
        uint8_t buf_byte = ((uint8_t*)buffer)[i];
        uint8_t mem_byte = eeprom_read_byte(addr);
        if (mem_byte != buf_byte) eeprom_write_byte(addr, buf_byte);
    }
    return true;
}

bool safe_eeprom_read_byte(size_t offset, size_t idx, uint8_t* byte) {
    if (!check_args(offset, idx) || !check_magic_number(offset)) return false;
    *byte = eeprom_read_byte(EEPROM_ADDR(offset, idx));
    return true;
}

bool safe_eeprom_write_byte(size_t offset, size_t idx, uint8_t byte) {
    if (!check_args(offset, idx) || !check_magic_number(offset)) return false;
    uint8_t* addr = EEPROM_ADDR(offset, idx);
    uint8_t mem_byte = eeprom_read_byte(addr);
    if (mem_byte != byte) eeprom_write_byte(addr, byte);
    return true;
}