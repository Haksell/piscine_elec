#include "main.h"

static void weather_report(t_aht20 aht20) {
    for (int i = 0; i < AHT20_BYTES; ++i) { print_hex_value(aht20[i]); }
    uart_printstrln("");
}

int main() {
    t_aht20 aht20;
    uart_init();
    i2c_init();
    aht20_init();
    while (true) {
        aht20_trigger_measurement();
        _delay_ms(AHT20_MEASURE_DELAY);
        switch (aht20_read_sensor(aht20)) {
            case AHT20_SENSOR_SUCCESS: weather_report(aht20); break;
            case AHT20_SENSOR_BUSY: uart_printstrln("AHT20 busy"); break;
            case AHT20_SENSOR_CRC_FAIL: uart_printstrln("CRC failure"); break;
        }
        _delay_ms(1000);
    }
}
