OSNAME=calcos

BUILD_FOLDER=build
SOURCE_FOLDER=src
AVR_OUTPUT_FORMAT=ihex

CPU=atmega328p
CPU_F=16000000 # 16 MHz

FLASH_BAUD=9600
FLASHER=avrdude

CC=avr-gcc
OBJCOPY=avr-objcopy
CSTANDARD=99
CDEFS=-DCPU=$(CPU) -DF_CPU=$(CPU_F) -DCALCOS_AVR

SRC=$(SOURCE_FOLDER)/gfx.c \
	$(SOURCE_FOLDER)/screen.c \
	$(SOURCE_FOLDER)/font.c \
	$(SOURCE_FOLDER)/icons.c \
	$(SOURCE_FOLDER)/screens/home_screen.c \
	$(SOURCE_FOLDER)/ui/menu.c

INCLUDES=-I$(SOURCE_FOLDER)/ \
		 -I$(SOURCE_FOLDER)/screens/ \
		 -I$(SOURCE_FOLDER)/ui/

create:
	mkdir -p $(BUILD_FOLDER)

avr: create
	$(CC) -mmcu=$(CPU) $(CDEFS) -std=gnu$(CSTANDARD) -Os -Wall $(SOURCE_FOLDER)/main-avr.c $(SRC) $(INCLUDES) -o $(BUILD_FOLDER)/$(OSNAME).elf
	$(OBJCOPY) -j .text -j .data -O $(AVR_OUTPUT_FORMAT) $(BUILD_FOLDER)/$(OSNAME).elf $(BUILD_FOLDER)/$(OSNAME).hex

clean:
	rm -f ./$(BUILD_FOLDER)