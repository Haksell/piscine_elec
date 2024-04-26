#include "apa102.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>

#define RAINBOW_PHASE_SHIFT (-30)

t_rgb colors[NUM_LEDS];
bool full_rainbow = false;
uint8_t rainbow_state = 0;

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

static t_rgb wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        return (t_rgb){255 - pos * 3, 0, pos * 3};
    } else if (pos < 170) {
        pos = pos - 85;
        return (t_rgb){0, pos * 3, 255 - pos * 3};
    } else {
        pos = pos - 170;
        return (t_rgb){pos * 3, 255 - pos * 3, 0};
    }
}

ISR(TIMER1_OVF_vect) {
    if (full_rainbow) {
        apa102_send_colors((t_rgb[NUM_LEDS]
        ){wheel(rainbow_state),
          wheel(rainbow_state + RAINBOW_PHASE_SHIFT),
          wheel(rainbow_state + (2 * RAINBOW_PHASE_SHIFT))});
    }
    ++rainbow_state;
}

static void setup_timer1() {
    TCCR1A = 0;
    TCCR1B = 1 << CS10;
    TIMSK1 = 1 << TOIE1;
}

int main() {
    uart_init();
    setup_timer1();
    apa102_init();
    apa102_send_colors(colors);
    sei();
    while (true) {
        char buffer[12];
        uart_readline("#", buffer, sizeof(buffer));
        str_upper(buffer);
        t_rgb color;
        uint8_t led;
        if (str_equals(buffer, "FULLRAINBOW")) {
            full_rainbow = true;
        } else if (parse_command(buffer, &color, &led)) {
            full_rainbow = false;
            colors[led] = color;
            apa102_send_colors(colors);
        } else {
            uart_putstrln("Invalid command.");
        }
    }
}
