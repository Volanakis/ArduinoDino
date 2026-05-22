MCU      = atmega328p
F_CPU    = 16000000UL
TARGET   = dino-game
SRC      = main.c display.c game.c dino.S
OBJ      = $(SRC:.c=.o)
OBJ      := $(OBJ:.S=.o)

TOOLS    = C:\Users\Dimit\ArduinoDino\tools
PATH     := $(TOOLS)\bin;$(PATH)
CC       = avr-gcc
OBJCOPY  = avr-objcopy
SIZE     = avr-size
CFLAGS   = -std=c99 -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -Wall -Wextra
LDFLAGS  = -mmcu=$(MCU)

.PHONY: all clean flash hex

all: $(TARGET).hex

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET).elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^
	$(SIZE) $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(TARGET).hex
	avrdude -c arduino -p $(MCU) -P COM3 -b 115200 -U flash:w:$<

clean:
	-del /F /Q $(OBJ) $(TARGET).elf $(TARGET).hex 2>nul
