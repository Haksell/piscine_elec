#include "i2c.h"
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define PCA9555_ADDRESS 0x20
#define PORT0_OUTPUT_REG 0x02
#define PORT0_CONFIG_REG 0x06
#define LED_D9 (1 << 3)

int main() {
    bool toggle_on = true;
    i2c_init();
    i2c_send_cmd2(PCA9555_ADDRESS, PORT0_CONFIG_REG, ~LED_D9);
    while (true) {
        i2c_send_cmd2(PCA9555_ADDRESS, PORT0_OUTPUT_REG, toggle_on ? ~LED_D9 : LED_D9);
        toggle_on = !toggle_on;
        _delay_ms(500);
    }
}
