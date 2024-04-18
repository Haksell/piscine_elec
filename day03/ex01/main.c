#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define ROUND_DIV(dividend, divisor)                                                               \
    ({                                                                                             \
        typeof(divisor) _divisor = (divisor);                                                      \
        ((dividend) + (_divisor >> 1)) / _divisor;                                                 \
    })

static void uart_init(void) {
    UBRR0 = ROUND_DIV(F_CPU, 16 * UART_BAUDRATE) - 1;
    UCSR0B |= 1 << TXEN0;
}

static void uart_tx(char c) {
    while (!(UCSR0A & 1 << UDRE0)) {}
    UDR0 = c;
}

static void uart_printstr(const char* str) {
    while (*str) uart_tx(*str++);
}

static void uart_printstrln(const char* str) {
    uart_printstr(str);
    uart_tx('\r');
    uart_tx('\n');
}

const float DUTY_STEP = 256.0 / F_CPU;
volatile float dutyCycle = 0;
volatile char c = 'A';
bool increasing = true;

ISR(TIMER0_OVF_vect) {
    dutyCycle += DUTY_STEP;
    if (dutyCycle >= 1) {
        uart_tx(c++);
        if (c > 'Z') c = 'A';
        uart_printstrln("ello World!"); // TODO: H
        dutyCycle = 0;
    }
}

void setup_timer0() {
    TCCR0A = 0;
    TCCR0B = (TCCR0B & 0b11111000) | (1 << CS00);
    TIMSK0 |= 1 << TOIE0;
}

int main() {
    uart_init();
    setup_timer0();
    sei();
    while (true) {}
}
