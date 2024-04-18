#include "main.h"

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

bool login() {
    uart_printstrln("Enter your login:");
    bool good_username = read_string("username", USERNAME, false);
    bool good_password = read_string("password", PASSWORD, true);
    return good_username && good_password;
}