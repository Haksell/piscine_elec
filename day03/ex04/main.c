// TODO: buffer to check valid username/password combination
// TODO: handle delete
// TODO: light show

#include <avr/io.h>
#include <stdbool.h>

#define CRLF "\r\n"

#define ROUND_DIV(dividend, divisor)                                                               \
    ({                                                                                             \
        typeof(divisor) _divisor = (divisor);                                                      \
        ((dividend) + (_divisor >> 1)) / _divisor;                                                 \
    })

static char is_printable(char c) { return c >= 32 && c <= 126; }

static void uart_init(void) {
    UBRR0 = ROUND_DIV(F_CPU, 16 * UART_BAUDRATE) - 1;
    UCSR0B |= 1 << RXEN0 | 1 << TXEN0;
}

static void uart_tx(char c) {
    while (!(UCSR0A & 1 << UDRE0)) {}
    UDR0 = c;
}

static char uart_rx() {
    while (!(UCSR0A & 1 << RXC0)) {}
    return UDR0;
}

static void uart_printstr(const char* str) {
    while (*str) uart_tx(*str++);
}

static void uart_printstrln(const char* str) {
    uart_printstr(str);
    uart_printstr(CRLF);
}

static void read_string(char* prompt, bool hide) {
    uart_printstr("    ");
    uart_printstr(prompt);
    uart_printstr(": ");
    while (true) {
        char c = uart_rx();
        if (c == '\r') {
            uart_printstr(CRLF);
            return;
        }
        if (is_printable(c)) uart_tx(hide ? '*' : c);
    }
}

static bool login() {
    uart_printstrln("Enter your login:");
    read_string("username", false);
    read_string("password", true);
    uart_printstr("Bad combination username/password" CRLF CRLF);
    return false;
}

static void light_show() {
    uart_printstr(CRLF "gg ez" CRLF);
    while (true) {}
}

int main() {
    uart_init();
    while (!login()) {}
    light_show();
}
