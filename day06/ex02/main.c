#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>

volatile uint8_t pos = 0;

static void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    OCR0B = r;
    OCR0A = g;
    OCR2B = b;
}

static void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        set_rgb(255 - pos * 3, 0, pos * 3);
    } else if (pos < 170) {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    } else {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

static void setup_timer0() {
    TCCR0A = 1 << WGM00 | 1 << WGM01 | 1 << COM0A1 | 1 << COM0B1;
    TCCR0B = 1 << CS00;
}

ISR(TIMER1_OVF_vect) { wheel(++pos); }

static void setup_timer1() {
    TCCR1A = 0;
    TCCR1B = 1 << CS10;
    TIMSK1 = 1 << TOIE1;
}

static void setup_timer2() {
    TCCR2A = 1 << WGM20 | 1 << WGM21 | 1 << COM2B1;
    TCCR2B = 1 << CS20;
}

static void init_rgb() {
    DDRD = 0b1101000;
    PORTD = 0;
    setup_timer0();
    setup_timer1();
    setup_timer2();
    wheel(0);
    sei();
}

int main() {
    init_rgb();
    while (true) {}
}
