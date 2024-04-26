#include "i2c.h"
#include <avr/io.h>
#include <stdbool.h>

#define PCA9555_ADDRESS 0x20

#define PCA9555_OUTPUT_REG 0x02
#define PCA9555_CONFIG_REG 0x06

#define DIGIT2 0b1011011

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
    pca9555_write(PCA9555_CONFIG_REG, (uint8_t)~LCD4, 0x00);
    pca9555_write(PCA9555_OUTPUT_REG, 0, DIGIT2);
}
