MCU := atmega328p
TARGET := main
HEX := $(TARGET).hex
BIN := $(TARGET).bin
F_CPU := 16000000UL
UART_BAUDRATE := 115200
PORT_FILE := /dev/ttyUSB0

all: hex flash

screen:
	screen $(PORT_FILE) $(UART_BAUDRATE)

killscreen:
	@killall screen 2> /dev/null || true

run: all screen

hex: $(HEX)

$(HEX): $(BIN)
	avr-objcopy -O ihex $< $@

$(BIN): $(TARGET).c
	avr-gcc -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DUART_BAUDRATE=$(UART_BAUDRATE) -Os -o $@ $<

flash: $(HEX) killscreen
	avrdude -c arduino -P $(PORT_FILE) -b $(UART_BAUDRATE) -p $(MCU) -U flash:w:$<:i

clean: killscreen
	rm -f $(BIN) $(HEX)

.PHONY: all screen killscreen run hex flash clean