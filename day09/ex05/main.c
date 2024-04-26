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

#define LCD1 (1 << 4)
#define LCD2 (1 << 5)
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

static void seven_segment_show(uint8_t pin, uint8_t digit) {
    pca9555_write(PCA9555_CONFIG_REG, (uint8_t)~pin, 0);
    pca9555_write(PCA9555_OUTPUT_REG, 0, DIGITS[digit]);
    pca9555_write(PCA9555_OUTPUT_REG, 0, 0);
}

int main() {
    i2c_init();
    uint16_t counter = 0;
    while (true) {
        uint8_t d1 = counter / 1000;
        uint8_t d2 = counter / 100 % 10;
        uint8_t d3 = counter / 10 % 10;
        uint8_t d4 = counter % 10;
        // best I can do without interrupts
        for (uint8_t i = 0; i < 218; ++i) {
            seven_segment_show(LCD1, d1);
            seven_segment_show(LCD2, d2);
            seven_segment_show(LCD3, d3);
            seven_segment_show(LCD4, d4);
        }
        counter = (counter + 1) % 10000;
    }
}
