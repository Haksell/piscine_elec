// TODO: print bytes in UART?
// The program must return status values to your computer after each data transmission.

#include <avr/io.h>
#include <util/twi.h>

#define AHT20 0x38

void i2c_init() {
    TWSR = 0x00; // set prescaler to 1
    TWBR = (uint8_t)((F_CPU / F_SCL - 16) / 2);
}

void i2c_write(uint8_t byte) {
    TWDR = byte;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & 1 << TWINT)) {}
}

void i2c_start(uint8_t address) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
    while (!(TWCR & 1 << TWINT)) {}
    i2c_write(address << 1); // TODO: "| 0" or "| 1" depending on read or write
}

void i2c_stop() {
    TWCR = 1 << TWSTO | 1 << TWEN | 1 << TWINT;
    while (!(TWCR & 1 << TWSTO)) {}
}

int main() {
    i2c_init();
    i2c_start(AHT20);
    i2c_stop();
    return 0;
}
