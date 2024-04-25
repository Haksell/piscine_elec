#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>
#include <util/delay.h>

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} t_rgb;

#define SS (1 << PB2)
#define MOSI (1 << PB3)
#define SCK (1 << PB5)

static void spi_init_master() {
    DDRB = SS | MOSI | SCK; // no idea why we need SS
    SPCR = 1 << SPE | 1 << MSTR | 1 << SPR0; // enable SPI, master, prescaler 16
}

static void spi_send(uint8_t data) {
    SPDR = data;
    while (!(SPSR & 1 << SPIF)) {}
}

static void apa102_start() {
    spi_send(0x00);
    spi_send(0x00);
    spi_send(0x00);
    spi_send(0x00);
}

static void apa102_send_color(t_rgb rgb) {
    spi_send(0xe3); // 20% brightness
    spi_send(rgb.blue);
    spi_send(rgb.green);
    spi_send(rgb.red);
}

static void apa102_end() {
    spi_send(0xff);
    spi_send(0xff);
    spi_send(0xff);
    spi_send(0xff);
}

#define NUM_LEDS 3

#define RGB_BLACK ((t_rgb){0, 0, 0})
#define RGB_DODGER_BLUE ((t_rgb){0x1e, 0x90, 0xff})

int main() {
    spi_init_master();
    uint8_t i = 0;
    while (true) {
        apa102_start();
        for (uint8_t j = 0; j < NUM_LEDS; ++j)
            apa102_send_color(i == j ? RGB_DODGER_BLUE : RGB_BLACK);
        apa102_end();
        i = (i + 1) % (NUM_LEDS + 1);
        _delay_ms(250);
    }
}
