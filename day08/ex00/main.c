#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

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

static void apa102_color(uint8_t red, uint8_t green, uint8_t blue) {
    spi_send(0xe4); // 25% brightness
    spi_send(blue);
    spi_send(green);
    spi_send(red);
}

static void apa102_end() {
    spi_send(0xff);
    spi_send(0xff);
    spi_send(0xff);
    spi_send(0xff);
}

int main() {
    spi_init_master();
    apa102_start();
    apa102_color(0xff, 0, 0);
    apa102_color(0, 0, 0);
    apa102_color(0, 0, 0);
    apa102_end();
    while (true) {}
}
