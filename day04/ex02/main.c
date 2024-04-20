#include "main.h"

#define ROLLING_AVERAGE 3

static void weather_report(aht20_t aht20) {
    // static uint32_t i = 0;
    // static float temps[ROLLING_AVERAGE];
    // static float hums[ROLLING_AVERAGE];
    // TODO: avg of last 3
    float temp = aht20_get_temperature(aht20);
    float hum = aht20_get_humidity(aht20);
    uart_printstr("Temperature: ");
    uart_printfloat(temp);
    uart_printstr("°C, Humidity: ");
    uart_printfloat(hum * 100);
    uart_printstrln("%");
}

int main() {
    aht20_t aht20;
    uart_init();
    i2c_init();
    aht20_init();
    while (true) {
        aht20_trigger_measurement();
        _delay_ms(AHT20_MEASURE_DELAY);
        if (aht20_read_cmd() & AHT20_BUSY) uart_printstrln("AHT20 busy");
        else if (aht20_read_sensor(&aht20)) weather_report(aht20);
        else uart_printstrln("CRC failure");
        _delay_ms(1000);
    }
}
