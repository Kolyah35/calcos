OSNAME=calcos

BUILD_DIR=build
SRC_DIR=./src
AVR_OUTPUT_FORMAT=ihex
OUTPUT_FILE=$(BUILD_DIR)/$(OSNAME)
RAYLIB_PATH=./libraries/raylib

CPU=atmega328p
CPU_F=16000000 # 16 MHz

FLASH_PORT=COM6
FLASH_BAUD=115200
FLASHER=avrdude

CC_AVR=avr-gcc
CC_SIM=gcc
OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump

CSTANDARD=99
CDEFS=-DCPU=$(CPU) -DF_CPU=$(CPU_F)
COPTS=-Os -Wall -std=gnu$(CSTANDARD) -Wno-missing-braces
LDFLAGS=-L$(SRC_DIR) -L$(RAYLIB_PATH)/src
LDLIBS = -lopengl32 -lgdi32 -lwinmm

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SRC=$(SRC_DIR)/gfx.c \
	$(SRC_DIR)/screen.c \
	$(SRC_DIR)/font.c \
	$(SRC_DIR)/icons.c \
	$(SRC_DIR)/screens/home_screen.c \
	$(SRC_DIR)/ui/menu.c 

SRC_AVR=$(SRC_DIR)/drv/digital.c \
		$(SRC_DIR)/drv/analog.c \
		$(SRC_DIR)/drv/spi.c \
		$(SRC_DIR)/drv/uart.c \
		$(SRC_DIR)/drv/display_st7565.c

RAYLIB_SRC = $(call rwildcard, $(RAYLIB_PATH)/src/*.c, $(RAYLIB_PATH)/src/*.h)

INCLUDES=-I$(SRC_DIR)/ \
		 -I$(SRC_DIR)/screens/ \
		 -I$(SRC_DIR)/ui/

INCLUDES_AVR=$(INCLUDES) \
			 -I$(SRC_DIR)/drv \

INCLUDES_SIM=$(INCLUDES) \
			 -I$(RAYLIB_PATH)/src \
			 -I$(RAYLIB_PATH)/src/external

create:
#	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	./build-options.bat

avr: create
	$(CC_AVR) -mmcu=$(CPU) $(CDEFS) -DPLATFORM_AVR $(COPTS) $(SRC_DIR)/main-avr.c $(SRC) $(SRC_AVR) $(INCLUDES_AVR) -o $(OUTPUT_FILE).elf
	$(OBJCOPY) -j .text -j .data -O $(AVR_OUTPUT_FORMAT) $(OUTPUT_FILE).elf $(OUTPUT_FILE).hex
	$(OBJCOPY) -j .text -j .data -O binary $(OUTPUT_FILE).elf $(OUTPUT_FILE).bin

sim: create
	echo $(RAYLIB_SRC)
	$(CC_SIM) $(CDEFS) -DPLATFORM_SIM $(COPTS) $(SRC_DIR)/main-sim.c $(SRC) $(RAYLIB_SRC) $(INCLUDES_SIM) $(LDFLAGS) $(LDLIBS) -o $(OSNAME).exe

all: avr deploy

clean:
	rm -f ./$(BUILD_DIR)

dump:
	$(OBJDUMP) -h -S $(OUTPUT_FILE).elf > $(OUTPUT_FILE).lst

deploy:
	avrdude -c arduino -F -P $(FLASH_PORT) -p $(CPU) -b $(FLASH_BAUD) -U flash:w:$(OUTPUT_FILE).hex:i