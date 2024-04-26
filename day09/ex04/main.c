#include "i2c.h"
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define PCA9555_ADDRESS 0x20

#define PCA9555_OUTPUT_REG 0x02
#define PCA9555_CONFIG_REG 0x06

const uint8_t DIGITS[10] = {
    0b0111111,
    0b0000110,
    0b1011011,
    0b1001111,
    0b1100110,
    0b1101101,
    0b1111101,
    0b0000111,
    0b1111111,
    0b1101111
};

#define LCD3 (1 << 6)
#define LCD4 (1 << 7)

static void pca9555_write(uint8_t reg, uint8_t data0, uint8_t data1) {
    i2c_start();
    i2c_write(PCA9555_ADDRESS << 1, true);
    i2c_write(reg, true);
    i2c_write(data0, true);
    i2c_write(data1, true);
    i2c_stop();
}

int main() {
    i2c_init();
    while (true) {
        pca9555_write(PCA9555_CONFIG_REG, (uint8_t)~LCD3, 0x00);
        pca9555_write(PCA9555_OUTPUT_REG, 0, DIGITS[4]);
        _delay_us(100);
        pca9555_write(PCA9555_OUTPUT_REG, 0, 0);
        _delay_us(100);
        pca9555_write(PCA9555_CONFIG_REG, (uint8_t)~LCD4, 0x00);
        pca9555_write(PCA9555_OUTPUT_REG, 0, DIGITS[2]);
        _delay_us(100);
        pca9555_write(PCA9555_OUTPUT_REG, 0, 0);
        _delay_us(100);
    }
}
