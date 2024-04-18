#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>

#define CTC_MODE (1 << WGM12)
#define PRESCALER_1024 (1 << CS10 | 1 << CS12)
#define PRINT_INTERVAL 2.0 // if too big, will overflow OCR1A

#define ROUND_DIV(dividend, divisor)                                                               \
    ({                                                                                             \
        typeof(divisor) _divisor = (divisor);                                                      \
        ((dividend) + (_divisor >> 1)) / _divisor;                                                 \
    })

static void uart_init() {
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

ISR(TIMER1_COMPA_vect) { uart_printstrln("Hello World!"); }

void setup_timer1() {
    TCCR1B |= CTC_MODE | PRESCALER_1024;
    OCR1A = F_CPU * PRINT_INTERVAL / 1024 - 1;
    TIMSK1 |= 1 << OCIE1A;
}

int main() {
    uart_init();
    setup_timer1();
    sei();
    while (true) {}
}
