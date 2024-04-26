#include "i2c.h"
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define PCA9555_ADDRESS 0x20

#define PCA9555_INPUT_REG 0x00
#define PCA9555_OUTPUT_REG 0x02
#define PCA9555_CONFIG_REG 0x06

#define BUTTON_SW3 (1 << 0)
#define LED_D11 (1 << 1)
#define LED_D10 (1 << 2)
#define LED_D9 (1 << 3)
#define LEDS (LED_D11 | LED_D10 | LED_D9)

static void pca9555_write(uint8_t reg, uint8_t data0, uint8_t data1) {
    i2c_start();
    i2c_write(PCA9555_ADDRESS << 1, true);
    i2c_write(reg, true);
    i2c_write(data0, true);
    i2c_write(data1, true);
    i2c_stop();
}

static uint8_t pca9555_read(uint8_t reg) {
    i2c_start();
    i2c_write(PCA9555_ADDRESS << 1, true);
    i2c_write(reg, true);
    i2c_start();
    i2c_write(PCA9555_ADDRESS << 1 | 1, true);
    uint8_t data = i2c_read(false);
    i2c_stop();
    return data;
}

int main() {
    i2c_init();
    pca9555_write(PCA9555_CONFIG_REG, ~LEDS, 0);
    while (true) {
        uint8_t port0_state = pca9555_read(PCA9555_INPUT_REG);
        bool button_pressed = !(port0_state & BUTTON_SW3);
        pca9555_write(PCA9555_OUTPUT_REG, button_pressed ? ~LEDS : LEDS, 0);
    }
}
