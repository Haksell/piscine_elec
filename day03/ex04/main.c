#include "main.h"

int main() {
    uart_init();
    while (!login()) uart_printstrln("Bad combination username/password" CRLF);
    uart_printstrln("Hello spectre!");
    uart_printstrln("Shall we play a game?");
    light_show();
}
