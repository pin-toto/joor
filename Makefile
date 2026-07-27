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

kernel.elf: $(C_OBJECTS) $(ASM_OBJECTS)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/kernel.elf $^

kernel.bin: kernel.elf
	objcopy -O binary $(BUILD_DIR)/kernel.elf $@

clean:
	rm -rf $(BUILD_DIR) kernel.bin os.iso iso
	@echo "🧹 Cleaned"

# ======== ISO با grub-mkrescue (همیشه کار میکنه) ========
iso: kernel.elf
	mkdir -p iso/boot/grub
	cp $(BUILD_DIR)/kernel.elf iso/boot/kernel.elf
	cp boot/grub/grub.cfg iso/boot/grub/
	@echo "📀 Creating ISO with grub-mkrescue..."
	grub-mkrescue -o os.iso iso 2>/dev/null || true
	@echo "✅ ISO created: os.iso"
	@echo "📦 Size:"
	@du -h os.iso

run: iso
	qemu-system-i386 -cdrom os.iso -vga std -m 32 -no-reboot

run-elf: kernel.elf
	qemu-system-i386 -kernel $(BUILD_DIR)/kernel.elf -vga std -m 32 -no-reboot

.PHONY: all clean iso run run-elf directories
