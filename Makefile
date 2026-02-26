CC = gcc
LD = ld
NASM = nasm

CFLAGS = -m32 -ffreestanding -fno-builtin -nostdlib -fno-pic -I src
LDFLAGS = -m elf_i386 -T src/kernel/linker.ld

BUILD = build
SRC = src

.PHONY: all clean run

all: $(BUILD)/solos.img

# Assemble bootloader
$(BUILD)/boot.bin: $(SRC)/boot/bootloader.asm
	$(NASM) -f bin $< -o $@

# Assemble kernel entry
$(BUILD)/multiboot.o: $(SRC)/boot/multiboot.asm
	$(NASM) -f elf32 $< -o $@

$(BUILD)/kernel_entry.o: $(SRC)/boot/kernel_entry.asm
	$(NASM) -f elf32 $< -o $@

# Compile C files
$(BUILD)/kernel.o: $(SRC)/kernel/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/shell.o: $(SRC)/kernel/shell.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/utils.o: $(SRC)/kernel/utils.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel ELF
$(BUILD)/kernel.elf: $(BUILD)/multiboot.o $(BUILD)/kernel_entry.o $(BUILD)/kernel.o $(BUILD)/shell.o $(BUILD)/utils.o
	$(LD) $(LDFLAGS) -o $@ $^

# Strip to raw binary
$(BUILD)/kernel.bin: $(BUILD)/kernel.elf
	objcopy -O binary --set-section-flags .bss=alloc,load,contents $< $@

# Build disk image
$(BUILD)/solos.img: $(BUILD)/boot.bin $(BUILD)/kernel.bin
	cat $(BUILD)/boot.bin $(BUILD)/kernel.bin > $(BUILD)/solos.img

# Run in QEMU
run: $(BUILD)/solos.img
	qemu-system-i386 -drive format=raw,file=$(BUILD)/solos.img,if=floppy -boot a

clean:
	rm -f $(BUILD)/*.o $(BUILD)/*.bin $(BUILD)/*.elf $(BUILD)/*.iso $(BUILD)/*.img