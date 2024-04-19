MCU := atmega328p
TARGET := main
HEX := $(TARGET).hex
BIN := $(TARGET).bin
PORT_FILE := /dev/ttyUSB0

F_CPU := 16000000UL
F_SCL := 100000UL
UART_BAUDRATE := 115200
DEFINES := -DF_CPU=$(F_CPU) -DF_SCL=$(F_SCL) -DUART_BAUDRATE=$(UART_BAUDRATE)

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
	avr-gcc -mmcu=$(MCU) $(DEFINES) -Os -o $@ $<

flash: $(HEX) killscreen
	avrdude -c arduino -P $(PORT_FILE) -b $(UART_BAUDRATE) -p $(MCU) -U flash:w:$<:i

clean: killscreen
	rm -f $(BIN) $(HEX)

.PHONY: all screen killscreen run hex flash clean