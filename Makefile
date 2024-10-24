include ./platform.mk

OSNAME=calcos

BUILD_DIR=build
SRC_DIR=./src
AVR_OUTPUT_FORMAT=ihex
OUTPUT_FILE=$(BUILD_DIR)/$(OSNAME)
RAYLIB_PATH=./libraries/raylib

CPU=atmega328p
CPU_F=16000000 # 16 MHz

FLASH_PORT=COM7
FLASH_BAUD=115200
FLASHER=avrdude

CC_AVR=avr-gcc
CC_SIM=gcc
OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump

PLATFORM_LIBS=
EXECUTABLE_FORMAT=

CSTANDARD=99
CDEFS=-DCPU=$(CPU) -DF_CPU=$(CPU_F)
COPTS=-Os -Wall -std=gnu$(CSTANDARD) -Wno-missing-braces -ggdb
LDFLAGS=-L$(SRC_DIR)

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

ifeq ($(SYSTEM),WIN32)
	PLATFORM_LIBS=-lopengl32 -lgdi32 -lwinmm
	EXECUTABLE_FORMAT=.exe
endif
ifeq ($(SYSTEM),LINUX)
	PLATFORM_LIBS=-L./libraries -lm
endif

LDLIBS = -lraylib $(PLATFORM_LIBS)

SRC=$(SRC_DIR)/main.c					\
	$(SRC_DIR)/gfx.c 					\
	$(SRC_DIR)/font.c 					\
	$(SRC_DIR)/icons.c 					\
	$(SRC_DIR)/screens/screen.c 		\
	$(SRC_DIR)/screens/home_screen.c 	\
	$(SRC_DIR)/screens/flasher_screen.c \
	$(SRC_DIR)/ui/node.c				\
	$(SRC_DIR)/ui/menu.c 				\
	$(SRC_DIR)/ui/image.c 				\
	$(SRC_DIR)/keyboard.c 				\
	$(SRC_DIR)/ui/text.c


SRC_AVR=$(SRC_DIR)/drv/digital.c \
		$(SRC_DIR)/drv/analog.c \
		$(SRC_DIR)/drv/spi.c \
		$(SRC_DIR)/drv/uart.c \
		$(SRC_DIR)/drv/display_st7565.c

SRC_SIM=$(SRC_DIR)/drv/display_sim.c


INCLUDES=-I$(SRC_DIR)/         \
		 -I$(SRC_DIR)/screens/ \
		 -I$(SRC_DIR)/ui/      \
		 -I$(SRC_DIR)/drv/

INCLUDES_SIM=$(INCLUDES)

create:
#	mkdir -p $(BUILD_DIR)

avr: create
	$(CC_AVR) -mmcu=$(CPU) $(CDEFS) -DPLATFORM_AVR $(COPTS) $(SRC) $(SRC_AVR) $(INCLUDES) -o $(OUTPUT_FILE).elf
	$(OBJCOPY) -j .text -j .data -O $(AVR_OUTPUT_FORMAT) $(OUTPUT_FILE).elf $(OUTPUT_FILE).hex
	$(OBJCOPY) -j .text -j .data -O binary $(OUTPUT_FILE).elf $(OUTPUT_FILE).bin

sim: create
	$(CC_SIM) $(CDEFS) -DPLATFORM_SIM $(COPTS) $(SRC) $(SRC_SIM) $(RAYLIB_SRC) $(INCLUDES_SIM) $(LDFLAGS) $(LDLIBS) -o $(BUILD_DIR)/$(OSNAME)$(EXECUTABLE_FORMAT)

all: avr deploy

clean:
	rm -f ./$(BUILD_DIR)

dump:
	$(OBJDUMP) -h -S $(OUTPUT_FILE).elf > $(OUTPUT_FILE).lst

deploy:
	avrdude -c arduino -F -P $(FLASH_PORT) -p $(CPU) -b $(FLASH_BAUD) -U flash:w:$(OUTPUT_FILE).hex:i
