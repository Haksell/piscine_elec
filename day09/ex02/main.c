#include "i2c.h"
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h> // TODO

#define PCA9555_ADDRESS 0x20

#define PCA9555_OUTPUT_REG 0x02
#define PCA9555_CONFIG_REG 0x06

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
    pca9555_write(PCA9555_CONFIG_REG, 0x0f, 0x00);
    uint8_t n = 0;
    while (true) {
        pca9555_write(PCA9555_OUTPUT_REG, 0, n);
        ++n;
        _delay_ms(50);
    }
}
