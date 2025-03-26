# Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size
RM = rm -f

# Project name
PROJECT = blink

# Hardware specs
MCU = cortex-m3
TARGET = stm32f103x8

# Paths
SRC_DIR = .
BUILD_DIR = build
INC_DIRS = .

# Source files
SRCS = \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/startup_stm32f103xb.s

# CMSIS headers
INC_DIRS += $(SRC_DIR)

# Compiler flags
CFLAGS = -mcpu=$(MCU) -mthumb -Wall -Og -g
CFLAGS += -D$(TARGET) -DSTM32F103xB
CFLAGS += $(addprefix -I,$(INC_DIRS))
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map

# Linker script
LDSCRIPT = STM32F103XX_FLASH.ld
CFLAGS += -T$(LDSCRIPT)

# Build rules
OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.c=.o)))
OBJS := $(OBJS:.s=.o)

all: $(BUILD_DIR)/$(PROJECT).elf

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(PROJECT).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
	$(OBJCOPY) -O ihex $@ $(BUILD_DIR)/$(PROJECT).hex
	$(OBJCOPY) -O binary $@ $(BUILD_DIR)/$(PROJECT).bin
	$(SIZE) $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	$(RM) -r $(BUILD_DIR)

flash: $(BUILD_DIR)/$(PROJECT).bin
	st-flash --reset write $< 0x08000000

erase:
	st-flash erase

.PHONY: all clean flash