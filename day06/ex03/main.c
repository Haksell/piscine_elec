#include "main.h"

static void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    OCR0B = 255 - r;
    OCR0A = 255 - g;
    OCR2B = 255 - b;
}

static void init_rgb() {
    DDRD = 0b1101000;
    PORTD = 0;
    TCCR0A = 1 << WGM00 | 1 << WGM01 | 1 << COM0A0 | 1 << COM0A1 | 1 << COM0B0 | 1 << COM0B1;
    TCCR0B = 1 << CS00;
    TCCR2A = 1 << WGM20 | 1 << WGM21 | 1 << COM2B0 | 1 << COM2B1;
    TCCR2B = 1 << CS20;
    set_rgb(0, 0, 0);
}

static bool parse_hexa(char c, uint8_t* h) {
    if ('0' <= c && c <= '9') {
        *h = c - '0';
        return true;
    }
    if ('A' <= c && c <= 'F') {
        *h = (c & 31) + 9;
        return true;
    }
    return false;
}

static bool parse_color(char* buffer, uint8_t* r, uint8_t* g, uint8_t* b) {
    uint8_t r1, r2, g1, g2, b1, b2;
    if (buffer[0] != '#' || !parse_hexa(buffer[1], &r1) || !parse_hexa(buffer[2], &r2) ||
        !parse_hexa(buffer[3], &g1) || !parse_hexa(buffer[4], &g2) || !parse_hexa(buffer[5], &b1) ||
        !parse_hexa(buffer[6], &b2))
        return false;
    *r = r1 << 4 | r2;
    *g = g1 << 4 | g2;
    *b = b1 << 4 | b2;
    return true;
}

static void str_upper(char* s) {
    while (*s) {
        if ('a' <= *s && *s <= 'z') *s &= ~32;
        ++s;
    }
}

int main() {
    uart_init();
    init_rgb();
    while (true) {
        char buffer[8];
        uart_readline(buffer, sizeof(buffer));
        str_upper(buffer);
        uint8_t r, g, b;
        if (parse_color(buffer, &r, &g, &b)) {
            set_rgb(r, g, b);
            uart_putstr("Color set to ");
            uart_putstr(buffer);
            uart_putstrln(".");
        } else uart_putstrln("Invalid color.");
    }
}
