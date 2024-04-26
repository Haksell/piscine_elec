#include "i2c.h"

void i2c_init() { TWBR = (F_CPU / I2C_F - 16) / 2; }

void i2c_start() {
    TWCR = 1 << TWINT | 1 << TWEN | 1 << TWSTA;
    while (!(TWCR & 1 << TWINT)) {}
}

void i2c_stop() { TWCR = 1 << TWINT | 1 << TWEN | 1 << TWSTO; }

void i2c_write(uint8_t byte) {
    TWDR = byte;
    TWCR = 1 << TWINT | 1 << TWEN;
    while (!(TWCR & 1 << TWINT)) {}
}

uint8_t i2c_read() {
    TWCR = 1 << TWINT | 1 << TWEN;
    while (!(TWCR & 1 << TWINT)) {}
    return TWDR;
}

uint8_t i2c_read_ack() {
    TWCR = 1 << TWINT | 1 << TWEN | 1 << TWEA;
    while (!(TWCR & 1 << TWINT)) {}
    return TWDR;
}

void i2c_send_cmd1(uint8_t address, uint8_t cmd1) {
    i2c_start();
    i2c_write(address << 1 | I2C_WRITE);
    i2c_write(cmd1);
    i2c_stop();
}

void i2c_send_cmd2(uint8_t address, uint8_t cmd1, uint8_t cmd2) {
    i2c_start();
    i2c_write(address << 1 | I2C_WRITE);
    i2c_write(cmd1);
    i2c_write(cmd2);
    i2c_stop();
}

void i2c_send_cmd3(uint8_t address, uint8_t cmd1, uint8_t cmd2, uint8_t cmd3) {
    i2c_start();
    i2c_write(address << 1 | I2C_WRITE);
    i2c_write(cmd1);
    i2c_write(cmd2);
    i2c_write(cmd3);
    i2c_stop();
}
