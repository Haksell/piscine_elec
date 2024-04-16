#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define DEBOUNCE_ITERATIONS 12
#define DUTY_STEPS 10

static void set_fast_pwm_mode() {
    TCCR1A |= 1 << WGM11;
    TCCR1B |= 1 << WGM12 | 1 << WGM13;
    ICR1 = F_CPU / 1024 - 1;
    TCCR1A |= 1 << COM1A1;
}

static bool check_button(uint8_t pin, int* counter) {
    *counter = (PIND >> pin) & 1 ? 0 : *counter + (*counter <= DEBOUNCE_ITERATIONS);
    return *counter == DEBOUNCE_ITERATIONS;
}

static void set_ocr1a(int duty) { OCR1A = duty * 0.1 * ICR1; }

int main() {
    DDRB = (1 << PB1);
    TCCR1A = 0;
    TCCR1B = 1 << CS10 | 1 << CS12;
    set_fast_pwm_mode();
    int duty = 1;
    set_ocr1a(duty);
    int counterSW1 = 0;
    int counterSW2 = 0;
    while (true) {
        if (check_button(PD2, &counterSW1) && duty < DUTY_STEPS) set_ocr1a(++duty);
        if (check_button(PD4, &counterSW2) && duty > 1) set_ocr1a(--duty);
        _delay_ms(1);
    }
    return 0;
}
