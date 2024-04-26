#include "i2c.h"

void i2c_init() { TWBR = (F_CPU / I2C_F - 16) / 2; }

void i2c_start() {
    TWCR = 1 << TWINT | 1 << TWEN | 1 << TWSTA;
    while (!(TWCR & 1 << TWINT)) {}
}

void i2c_stop() { TWCR = 1 << TWINT | 1 << TWEN | 1 << TWSTO; }

void i2c_write(uint8_t byte, bool ack) {
    TWDR = byte;
    TWCR = 1 << TWINT | 1 << TWEN | !!ack << TWEA;
    while (!(TWCR & 1 << TWINT)) {}
}

uint8_t i2c_read(bool ack) {
    TWCR = 1 << TWINT | 1 << TWEN | !!ack << TWEA;
    while (!(TWCR & 1 << TWINT)) {}
    return TWDR;
}
