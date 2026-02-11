ARCH        := esp32s3
BUILD_DIR   := ../build/$(ARCH)
SRC_DIR     := .
INC_DIR     := $(SRC_DIR)

TOOLCHAIN 	:= xtensa-esp32s3-elf
CC          := $(TOOLCHAIN)-gcc
AR          := $(TOOLCHAIN)-ar
AS			:= $(TOOLCHAIN)-as
LD          := $(CC)

QEMU		:= qemu-system-xtensa

CFLAGS      := -std=c11 -Wall -Wextra -ffreestanding -Os -I$(INC_DIR) -DARCH_ESP 
LDSCRIPT    := $(SRC_DIR)/arch/$(ARCH)/$(ARCH).ld
LDFLAGS     := -T$(LDSCRIPT) -nostdlib

C_SRCS      := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/arch/$(ARCH)/*.c) 
S_SRCS		:= $(SRC_DIR)/arch/$(ARCH)/start.S
OBJS 		:= $(patsubst $(SRC_DIR)/%.S,$(BUILD_DIR)/%.o,$(S_SRCS)) $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRCS))

all: esp32s3
esp32s3: $(BUILD_DIR)/calcos-$(ARCH).elf

qemu: $(BUILD_DIR)/calcos-$(ARCH).elf
	$(QEMU) \
		-machine esp32s3 \
		-m 512k \
		-serial mon:stdio \
		-kernel $<

$(BUILD_DIR)/calcos-$(ARCH).elf: $(OBJS)
	$(LD) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
