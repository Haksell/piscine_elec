MCU := atmega328p
TARGET := main
SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)
HEX := $(TARGET).hex
BIN := $(TARGET).bin
F_CPU := 16000000UL
UART_BAUDRATE := 115200
PORT_FILE := /dev/ttyUSB0
CC := avr-gcc
CFLAGS := -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DUART_BAUDRATE=$(UART_BAUDRATE) -Os

all: hex flash

screen:
	screen $(PORT_FILE) $(UART_BAUDRATE)

killscreen:
	@killall screen 2> /dev/null || true

run: all screen

hex: $(HEX)

$(HEX): $(BIN)
	avr-objcopy -O ihex $< $@

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c main.h
	$(CC) $(CFLAGS) -c $< -o $@

flash: $(HEX) killscreen
	avrdude -c arduino -P $(PORT_FILE) -b $(UART_BAUDRATE) -p $(MCU) -U flash:w:$<:i

clean: killscreen
	rm -f $(BIN) $(HEX) $(OBJ)

.PHONY: all screen killscreen run hex flash clean
