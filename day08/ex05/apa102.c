#include "apa102.h"

void apa102_init() {
    DDRB = 1 << PB2 | 1 << PB3 | 1 << PB5;
    SPCR = 1 << SPE | 1 << MSTR | 1 << SPR0;
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
    spi_send(0xe1);
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

void apa102_send_colors(t_rgb colors[NUM_LEDS]) {
    apa102_start();
    for (size_t i = 0; i < NUM_LEDS; ++i) apa102_send_color(colors[i]);
    apa102_end();
}
