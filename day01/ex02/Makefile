MCU = atmega328p
TARGET = main
HEX = $(TARGET).hex
BIN = $(TARGET).bin

all: hex flash

hex: $(HEX)

$(HEX): $(BIN)
	avr-objcopy -O ihex $(BIN) $(HEX)

$(BIN): $(TARGET).c
	avr-gcc -mmcu=$(MCU) -DF_CPU=16000000UL -Os -o $(BIN) $(TARGET).c

flash: $(HEX)
	avrdude -c arduino -P /dev/ttyUSB0 -b 115200 -p $(MCU) -U flash:w:$(HEX):i

clean:
	rm -f $(BIN) $(HEX)

.PHONY: all hex flash clean