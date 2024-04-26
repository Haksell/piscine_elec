#include "i2c.h"
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define PCA9555_ADDRESS 0x20
#define PCA9555_OUTPUT_REF 0x02
#define PCA9555_CONFIG_REF 0x06
#define LED_D9 (1 << 3)

static void pca9555_write_config(uint8_t data0, uint8_t data1) {
    i2c_start();
    i2c_write(PCA9555_ADDRESS << 1, true);
    i2c_write(PCA9555_CONFIG_REF, true);
    i2c_write(data0, true);
    i2c_write(data1, true);
    i2c_stop();
}

static void pca9555_write_output(uint8_t data0, uint8_t data1) {
    i2c_start();
    i2c_write(PCA9555_ADDRESS << 1, true);
    i2c_write(PCA9555_OUTPUT_REF, true);
    i2c_write(data0, true);
    i2c_write(data1, true);
    i2c_stop();
}

int main() {
    bool toggle_on = true;
    i2c_init();
    pca9555_write_config(~LED_D9, 0);
    while (true) {
        pca9555_write_output(toggle_on ? ~LED_D9 : LED_D9, 0);
        toggle_on = !toggle_on;
        _delay_ms(500);
    }
}
