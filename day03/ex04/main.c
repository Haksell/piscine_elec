// TODO: light show

#include "main.h"

#define USERNAME "spectre"
#define PASSWORD "oogachaka"

#define BACKSPACE '\x7f'
#define CRLF "\r\n"

#define ROUND_DIV(dividend, divisor)                                                               \
    ({                                                                                             \
        typeof(divisor) _divisor = (divisor);                                                      \
        ((dividend) + (_divisor >> 1)) / _divisor;                                                 \
    })

static char is_printable(char c) { return c >= 32 && c <= 126; }

static bool str_equal(const char* s1, const char* s2) {
    while (*s1 || *s2) {
        if (*s1++ != *s2++) return false;
    }
    return true;
}

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

static bool read_string(char* prompt, char* target, bool hide) {
    char buffer[128];
    int i = 0;
    uart_printstr("    ");
    uart_printstr(prompt);
    uart_printstr(": ");
    while (true) {
        char c = uart_rx();
        if (c == '\r') {
            buffer[i] = '\0';
            uart_printstr(CRLF);
            return str_equal(buffer, target);
        }
        if (c == BACKSPACE && i > 0) {
            --i;
            uart_tx('\b');
            uart_tx(' ');
            uart_tx('\b');
        }
        if (is_printable(c) && i < sizeof(buffer) - 1) {
            buffer[i] = c;
            ++i;
            uart_tx(hide ? '*' : c);
        }
    }
}

static bool login() {
    uart_printstrln("Enter your login:");
    bool good_username = read_string("username", USERNAME, false);
    bool good_password = read_string("password", PASSWORD, true);
    return good_username && good_password;
}

static void light_show() {
    while (true) {}
}

int main() {
    uart_init();
    while (!login()) uart_printstrln("Bad combination username/password" CRLF);
    uart_printstrln("Hello spectre!");
    uart_printstrln("Shall we play a game?");
    light_show();
}
