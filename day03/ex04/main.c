#include "main.h"

int main() {
    uart_init();
    while (!login()) uart_printstrln("Bad combination username/password" CRLF);
    uart_printstrln(CRLF "Hello " USERNAME ". Enjoy the light show!" CRLF);
    light_show();
}
