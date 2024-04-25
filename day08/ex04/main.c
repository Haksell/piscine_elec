#include "uart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} t_rgb;

#define NUM_LEDS 3
#define RAINBOW_PHASE_SHIFT 45

t_rgb colors[NUM_LEDS];
bool rainbow_mode = false;
uint8_t rainbow_state = 0;

static void spi_init_master() {
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
    spi_send(0xe2); // brightness
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

static void apa102_send_colors(t_rgb colors[NUM_LEDS]) {
    apa102_start();
    for (size_t i = 0; i < NUM_LEDS; ++i) apa102_send_color(colors[i]);
    apa102_end();
}

static void str_upper(char* s) {
    while (*s) {
        if ('a' <= *s && *s <= 'z') *s &= ~32;
        ++s;
    }
}

static bool str_equals(char* s1, char* s2) {
    while (*s1 || *s2) {
        if (*s1++ != *s2++) return false;
    }
    return true;
}

static bool parse_hex(char c, uint8_t* hex) {
    if (c >= '0' && c <= '9') {
        *hex = c - 48;
        return true;
    } else if (c >= 'A' && c <= 'F') {
        *hex = c - 55;
        return true;
    } else return false;
}

static bool parse_command(char* s, t_rgb* color, uint8_t* led) {
    uint8_t r1, r2, g1, g2, b1, b2;
    if (parse_hex(s[0], &r1) && parse_hex(s[1], &r2) && parse_hex(s[2], &g1) &&
        parse_hex(s[3], &g2) && parse_hex(s[4], &b1) && parse_hex(s[5], &b2) && s[6] == 'D' &&
        s[7] >= '6' && s[7] <= '8' && s[8] == '\0') {
        color->red = r1 << 4 | r2;
        color->green = g1 << 4 | g2;
        color->blue = b1 << 4 | b2;
        *led = s[7] - '6';
        return true;
    } else return false;
}

static void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        apa102_send_color((t_rgb){255 - pos * 3, 0, pos * 3});
    } else if (pos < 170) {
        pos = pos - 85;
        apa102_send_color((t_rgb){0, pos * 3, 255 - pos * 3});
    } else {
        pos = pos - 170;
        apa102_send_color((t_rgb){pos * 3, 255 - pos * 3, 0});
    }
}

ISR(TIMER1_OVF_vect) {
    apa102_start();
    wheel(rainbow_state);
    wheel(rainbow_state + RAINBOW_PHASE_SHIFT);
    wheel(rainbow_state + (2 * RAINBOW_PHASE_SHIFT));
    apa102_end();
    ++rainbow_state;
}

static void setup_timer1() {
    TCCR1A = 0;
    TCCR1B = 1 << CS10;
    TIMSK1 = 1 << TOIE1;
}

// === RAINBOW MODE END ===

int main() {
    uart_init();
    setup_timer1();
    spi_init_master();
    apa102_send_colors(colors);
    sei();
    while (true) {
        char buffer[12];
        uart_readline("#", buffer, sizeof(buffer));
        str_upper(buffer);
        t_rgb color;
        uint8_t led;
        if (str_equals(buffer, "FULLRAINBOW")) {
            rainbow_mode = true;
            uart_putstrln("RAINBOW TODO");
        } else if (parse_command(buffer, &color, &led)) {
            rainbow_mode = false;
            colors[led] = color;
            apa102_send_colors(colors);
        } else {
            uart_putstrln("Invalid command.");
        }
    }
}
