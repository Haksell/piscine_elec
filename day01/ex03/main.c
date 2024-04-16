#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define DEBOUNCE_ITERATIONS 12
#define DUTY_STEPS 10

static void set_wgm_mode() {
    TCCR1A |= 1 << WGM11;
    TCCR1B |= 1 << WGM12 | 1 << WGM13;
    TCCR1A |= 1 << COM1A1;
    TCCR1B |= 1 << CS10 | 1 << CS12;
    ICR1 = F_CPU / 1024 - 1;
}

static bool check_button(uint8_t pin, uint8_t* counter) {
    *counter = (PIND >> pin) & 1 ? 0 : *counter + (*counter <= DEBOUNCE_ITERATIONS);
    return *counter == DEBOUNCE_ITERATIONS;
}

static void set_ocr1a(uint8_t duty) { OCR1A = (float)duty / DUTY_STEPS * ICR1; }

int main() {
    DDRB = 1 << PB1;
    set_wgm_mode();
    uint8_t duty = 1;
    set_ocr1a(duty);
    uint8_t counterSW1 = 0;
    uint8_t counterSW2 = 0;
    while (true) {
        if (check_button(PD2, &counterSW1) && duty < DUTY_STEPS) set_ocr1a(++duty);
        if (check_button(PD4, &counterSW2) && duty > 1) set_ocr1a(--duty);
        _delay_ms(1);
    }
}
