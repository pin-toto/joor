CC = gcc
ASM = nasm
LD = ld

CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -I src/include -g
LDFLAGS = -m elf_i386 -T linker.ld

SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
C_OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES))

ASM_SOURCES = $(wildcard $(SRC_DIR)/*.asm)
ASM_OBJECTS = $(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.o,$(ASM_SOURCES))

all: directories kernel.bin

directories:
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | directories
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm | directories
	$(ASM) -f elf32 $< -o $@

kernel.bin: $(C_OBJECTS) $(ASM_OBJECTS)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/kernel.elf $^
	objcopy -O binary $(BUILD_DIR)/kernel.elf $@
	@echo "✅ Kernel built: kernel.bin"

clean:
	rm -rf $(BUILD_DIR) kernel.bin
	@echo "🧹 Cleaned"

run: all
	qemu-system-i386 -kernel $(BUILD_DIR)/kernel.elf -vga std -m 32 -no-reboot

debug: all
	qemu-system-i386 -s -S -kernel $(BUILD_DIR)/kernel.elf -vga std -m 32 &

.PHONY: all clean run debug directories
