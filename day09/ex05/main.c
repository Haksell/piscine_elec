#include "i2c.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>

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

uint16_t counter = 0;

ISR(TIMER1_COMPA_vect) { counter = (counter + 1) % 10000; }

static void timer1_init() {
    TCCR1B |= 1 << WGM12 | 1 << CS12 | 1 << CS10; // CTC mode, prescaler=1024
    OCR1A = F_CPU / 1024 - 1;
    TIMSK1 |= 1 << OCIE1A;
}

int main() {
    i2c_init();
    timer1_init();
    sei();
    while (true) {
        uint16_t copy = counter; // avoids data races
        seven_segment_show(LCD1, copy / 1000);
        seven_segment_show(LCD2, copy / 100 % 10);
        seven_segment_show(LCD3, copy / 10 % 10);
        seven_segment_show(LCD4, copy % 10);
    }
}
