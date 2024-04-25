MCU := atmega328p
TARGET := main
HEADERS := $(wildcard *.h)
SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)
HEX := $(TARGET).hex
BIN := $(TARGET).bin
PORT_FILE := /dev/ttyUSB0

F_CPU := 16000000UL
I2C_F := 100000UL
UART_BAUDRATE := 115200
DEFINES := -DF_CPU=$(F_CPU) -DI2C_F=$(I2C_F) -DUART_BAUDRATE=$(UART_BAUDRATE)

CC := avr-gcc
CFLAGS := -Wall -Wextra -Werror -mmcu=$(MCU) $(DEFINES) -Os

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

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

flash: $(HEX) killscreen
	avrdude -c arduino -P $(PORT_FILE) -b $(UART_BAUDRATE) -p $(MCU) -U flash:w:$<:i

clean: killscreen
	rm -f $(BIN) $(HEX) $(OBJ)

.PHONY: all screen killscreen run hex flash clean
