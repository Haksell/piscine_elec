#include "i2c.h"
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define PCA9555_ADDRESS 0x20
#define PCA9555_OUTPUT_REG 0x02
#define PCA9555_CONFIG_REG 0x06
#define LED_D9 (1 << 3)

// TODO: clean pca9555_write and pca9555_read everywhere once I've really understood
static void pca9555_write(uint8_t reg, uint8_t data0, uint8_t data1) {
    i2c_start();
    i2c_write(PCA9555_ADDRESS << 1, true);
    i2c_write(reg, true);
    i2c_write(data0, true);
    i2c_write(data1, true);
    i2c_stop();
}

int main() {
    bool toggle_on = true;
    i2c_init();
    pca9555_write(PCA9555_CONFIG_REG, ~LED_D9, 0);
    while (true) {
        pca9555_write(PCA9555_OUTPUT_REG, toggle_on ? ~LED_D9 : LED_D9, 0);
        toggle_on = !toggle_on;
        _delay_ms(500);
    }
}
