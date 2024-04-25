#include "apa102.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>

#define RGB_BLACK ((t_rgb){0, 0, 0})
#define RGB_DODGER_BLUE ((t_rgb){0x1e, 0x90, 0xff})

ISR(ADC_vect) {
    uint8_t led = ADC * NUM_LEDS / 1023;
    t_rgb colors[NUM_LEDS];
    for (uint8_t i = 0; i < NUM_LEDS; ++i) colors[i] = i < led ? RGB_DODGER_BLUE : RGB_BLACK;
    apa102_send_colors(colors);
    ADCSRA |= 1 << ADSC;
}

static void adc_init() {
    ADMUX = 1 << REFS0 | PC0;
    ADCSRA = 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0 | 1 << ADIE | 1 << ADEN | 1 << ADSC;
}

int main() {
    adc_init();
    apa102_init();
    sei();
    while (true) {}
}
