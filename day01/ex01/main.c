// https://lab-allen.fr/atmega328-configuration-du-timer1/
// https://forum.arduino.cc/t/timer1-ctc-mode-and-set-clear-oc1a/598140/3

#include <avr/io.h>
#include <stdbool.h>

#define CTC_MODE (1 << WGM12) // CTC mode (Clear Timer on Compare Match)
#define PRESCALER_256 (1 << CS12) // how many cycles to increment the timer by 1

int main() {
    DDRB = 1 << PB1; // setup LED D2 as output
    TCCR1A = 1 << COM1A0; // toggle OC1A = PB1 on match
    TCCR1B = CTC_MODE | PRESCALER_256;
    OCR1A = F_CPU / (2 * 256) - 1; // perform action and reset timer when this value is reached
    while (true) {} // keep the program running
}
