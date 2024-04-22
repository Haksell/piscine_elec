#pragma once

void uart_init();
void uart_tx(char c);
char uart_rx();
void uart_printstr(const char* str);
void uart_printstrln(const char* str);
void uart_print_nibble(uint8_t byte);
void print_hex_value(uint8_t byte);
void uart_putnbr(uint32_t n);
void uart_printfloat(float x, int precision);
