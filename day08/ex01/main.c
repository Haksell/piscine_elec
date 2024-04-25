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

static void apa102_send_colors(t_rgb rgb6, t_rgb rgb7, t_rgb rgb8) {
    apa102_send_color(rgb6);
    apa102_send_color(rgb7);
    apa102_send_color(rgb8);
}

static void apa102_end() {
    spi_send(0xff);
    spi_send(0xff);
    spi_send(0xff);
    spi_send(0xff);
}

#define RGB_BLACK ((t_rgb){0, 0, 0})
#define RGB_WHITE ((t_rgb){0xff, 0xff, 0xff})
#define RGB_RED ((t_rgb){0xff, 0, 0})
#define RGB_YELLOW ((t_rgb){0xff, 0xff, 0})
#define RGB_GREEN ((t_rgb){0, 0xff, 0})
#define RGB_CYAN ((t_rgb){0, 0xff, 0xff})
#define RGB_BLUE ((t_rgb){0, 0, 0xff})
#define RGB_MAGENTA ((t_rgb){0xff, 0, 0xff})

t_rgb COLORS[] = {
    RGB_RED,
    RGB_GREEN,
    RGB_BLUE,
    RGB_YELLOW,
    RGB_CYAN,
    RGB_MAGENTA,
    RGB_WHITE,
};

int main() {
    const size_t num_colors = sizeof(COLORS) / sizeof(*COLORS);
    spi_init_master();
    uint8_t i = 0;
    while (true) {
        apa102_start();
        apa102_send_colors(COLORS[i], RGB_BLACK, RGB_BLACK);
        apa102_end();
        i = (i + 1) % num_colors;
        _delay_ms(1000);
    }
}
