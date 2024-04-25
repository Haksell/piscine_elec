#include "apa102.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>

#define DEBOUNCE_ITERATIONS 100

uint8_t rgb_index = 0;
uint8_t led_index = 0;

t_rgb colors[NUM_LEDS];

ISR(ADC_vect) {
    ((uint8_t*)&colors[led_index])[rgb_index] = ADCH;
    apa102_send_colors(colors);
    ADCSRA |= 1 << ADSC;
}

static void adc_init() {
    ADMUX = 1 << REFS0 | 1 << ADLAR | PC0;
    ADCSRA = 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0 | 1 << ADIE | 1 << ADEN | 1 << ADSC;
}

static bool is_pressed(uint8_t pin, int8_t* debounce) {
    *debounce = (PIND >> pin) & 1 ? DEBOUNCE_ITERATIONS : *debounce - (*debounce >= 0);
    return *debounce == 0;
}

int main() {
    adc_init();
    apa102_init();
    sei();
    DDRD &= ~(1 << PD2 | 1 << PD4);
    int8_t sw1 = -1, sw2 = -1;
    apa102_send_colors(colors);
    while (true) {
        if (is_pressed(PD2, &sw1)) rgb_index = (rgb_index + 1) % 3;
        if (is_pressed(PD4, &sw2)) led_index = (led_index + 1) % NUM_LEDS;
    }
}
