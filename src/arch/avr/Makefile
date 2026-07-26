ARCH        := avr
BUILD_DIR   := build/$(ARCH)
SRC_DIR     := src
INC_DIR     := $(SRC_DIR)

CC          := avr-gcc
OBJCOPY		:= avr-objcopy
LD          := $(CC)
QEMU		:= qemu-system-avr

DEVICE 		?= atmega328p
F_CPU		?= 16000000L
MACHINE		?= arduino-uno

CFLAGS      := -std=c11 -Wall -Wextra -ffreestanding -Os -I$(INC_DIR) -mmcu=$(DEVICE) -DF_CPU=$(F_CPU)
LDFLAGS     := -T$(SRC_DIR)/arch/$(ARCH)/linker.ld -Wl,--gc-sections

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

# C_SRCS      := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/arch/$(ARCH)/*.c)
C_SRCS		:= $(call rwildcard,$(SRC_DIR),*.c)
OBJS 		:= $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRCS))

# all: avr
avr: $(BUILD_DIR)/calcos-$(ARCH).bin

qemu:
	qemu-system-avr -machine $(MACHINE) -bios $(BUILD_DIR)/calcos-$(ARCH).elf

$(BUILD_DIR)/calcos-$(ARCH).bin: $(BUILD_DIR)/calcos-$(ARCH).elf
	$(OBJCOPY) -j .text -j .data -O binary $(BUILD_DIR)/calcos-$(ARCH).elf $(BUILD_DIR)/calcos-$(ARCH).bin

$(BUILD_DIR)/calcos-$(ARCH).elf: $(OBJS)
	$(LD) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
