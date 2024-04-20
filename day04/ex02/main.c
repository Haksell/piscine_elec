#include "main.h"

static void weather_report() {
    // TODO: avg of last 3
    float temp = aht20_get_temperature();
    float hum = aht20_get_humidity();
    uart_printstr("Temperature: ");
    uart_printfloat(temp);
    uart_printstr("°C, Humidity: ");
    uart_printfloat(hum * 100);
    uart_printstrln("%");
}

int main() {
    uart_init();
    i2c_init();
    aht20_init();
    while (true) {
        aht20_trigger_measurement();
        _delay_ms(AHT20_MEASURE_DELAY);
        if (aht20_read_cmd() & AHT20_BUSY) uart_printstrln("AHT20 busy");
        else if (aht20_read_sensor()) weather_report();
        else uart_printstrln("CRC failure");
        _delay_ms(1000);
    }
}
