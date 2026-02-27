#=========================================
# SolOS Makefile
# Fully automatic for all kernel C files (Option 2)
#=========================================

# Tools
CC      = gcc
LD      = ld
NASM    = nasm
OBJCOPY = objcopy

# Flags
CFLAGS  = -m32 -ffreestanding -fno-builtin -nostdlib -fno-pic -I src
LDFLAGS = -m elf_i386 -T src/kernel/linker.ld

# Directories
BUILD = build
SRC   = src

# Automatically find all kernel C source files
KERNEL_SRC := $(shell find $(SRC)/kernel -name "*.c")
KERNEL_OBJ := $(KERNEL_SRC:$(SRC)/%.c=$(BUILD)/%.o)

# Targets
.PHONY: all clean run

all: $(BUILD)/solos.img

#-----------------------------------------
# Assemble bootloader
$(BUILD)/boot.bin: $(SRC)/boot/bootloader.asm
	@mkdir -p $(BUILD)
	$(NASM) -f bin $< -o $@

# Assemble multiboot header
$(BUILD)/multiboot.o: $(SRC)/boot/multiboot.asm
	@mkdir -p $(BUILD)
	$(NASM) -f elf32 $< -o $@

# Assemble kernel entry
$(BUILD)/kernel_entry.o: $(SRC)/boot/kernel_entry.asm
	@mkdir -p $(BUILD)
	$(NASM) -f elf32 $< -o $@

#-----------------------------------------
# Compile all kernel C files automatically
$(BUILD)/%.o: $(SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

#-----------------------------------------
# Link kernel ELF
$(BUILD)/kernel.elf: $(BUILD)/multiboot.o $(BUILD)/kernel_entry.o $(KERNEL_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

# Strip to raw binary
$(BUILD)/kernel.bin: $(BUILD)/kernel.elf
	$(OBJCOPY) -O binary --set-section-flags .bss=alloc,load,contents $< $@

# Build disk image
$(BUILD)/solos.img: $(BUILD)/boot.bin $(BUILD)/kernel.bin
	cat $^ > $@

#-----------------------------------------
# Run in QEMU
run: $(BUILD)/solos.img
	qemu-system-i386 -drive format=raw,file=$(BUILD)/solos.img,if=floppy -boot a

#-----------------------------------------
# Clean build
clean:
	rm -rf $(BUILD)