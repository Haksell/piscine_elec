MCU = atmega328p
TARGET = main
HEX = $(TARGET).hex
BIN = $(TARGET).bin

all: hex flash

hex: $(HEX)

$(HEX): $(BIN)
	avr-objcopy -O ihex $< $@

$(BIN): $(TARGET).c
	avr-gcc -mmcu=$(MCU) -DF_CPU=16000000UL -Os -o $@ $<

killscreen:
	@killall screen 2> /dev/null || true

flash: $(HEX) killscreen
	avrdude -c arduino -P /dev/ttyUSB0 -b 115200 -p $(MCU) -U flash:w:$<:i

clean: killscreen
	rm -f $(BIN) $(HEX)

.PHONY: all hex flash clean killscreen