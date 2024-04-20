#include "main.h"

int main() {
    uart_init();
    i2c_init();
    aht20_init();
    while (true) {
        aht20_trigger_measurement();
        _delay_ms(AHT20_MEASURE_DELAY);
        aht20_process_measurement();
        _delay_ms(1000);
    }
}
