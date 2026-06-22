CC = gcc
CFLAGS = -m32 -ffreestanding -fno-builtin -fno-pic -fno-pie -nostdlib

OBJS = kernel.o vga.o keyboard.o ata.o idt.o inter.o timer.o gdt.o win.o

all: disk.img

# Assemble bootloader
boot.bin: boot.asm
	nasm -f bin boot.asm -o boot.bin

# Assemble interrupt handler
inter.o: inter.asm
	nasm -f elf32 inter.asm -o inter.o

# Compile C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel
kernel.elf: $(OBJS)
	ld -m elf_i386 -T link.ld -o kernel.elf $(OBJS)

# Convert to binary
kernel.bin: kernel.elf
	objcopy -O binary kernel.elf kernel.bin

# Build os.bin
os.bin: boot.bin kernel.bin
	cat boot.bin kernel.bin > os.bin

# Build disk image
disk.img: boot.bin kernel.bin
	dd if=/dev/zero of=disk.img bs=512 count=2880
	mkfs.fat -F 12 disk.img
	echo "I am a hacker" > h.txt
	mcopy -i disk.img h.txt ::h.txt
	dd if=boot.bin of=disk.img bs=446 count=1 conv=notrunc
	dd if=kernel.bin of=disk.img bs=512 seek=100 conv=notrunc

# Run QEMU
run: disk.img
	qemu-system-x86_64 -drive format=raw,file=disk.img -rtc base=localtime -vga std

# Clean everything
clean:
	rm -f *.o *.elf *.bin disk.img

.PHONY: all run clean
