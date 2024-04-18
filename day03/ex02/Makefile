MCU := atmega328p
TARGET := main
HEX := $(TARGET).hex
BIN := $(TARGET).bin
F_CPU := 16000000UL
UART_BAUDRATE := 115200

all: hex flash

screen: all
	screen /dev/ttyUSB0 $(UART_BAUDRATE)

hex: $(HEX)

$(HEX): $(BIN)
	avr-objcopy -O ihex $< $@

$(BIN): $(TARGET).c
	avr-gcc -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DUART_BAUDRATE=$(UART_BAUDRATE) -Os -o $@ $<

killscreen:
	@killall screen 2> /dev/null || true

flash: $(HEX) killscreen
	avrdude -c arduino -P /dev/ttyUSB0 -b $(UART_BAUDRATE) -p $(MCU) -U flash:w:$<:i

clean: killscreen
	rm -f $(BIN) $(HEX)

.PHONY: all screen hex killscreen flash clean