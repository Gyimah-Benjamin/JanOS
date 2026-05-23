# JanOS

A 32-bit x86 operating system built from scratch in C and x86 Assembly.

## Features

- Custom bootloader
- Protected mode
- Shell commands
- FAT12 filesystem support
- Disk read/write support
- Memory management and paging

## Commands

- ls — list files
- cat — read files  
- write — create files
- rm — delete files
- time — show current time
- date — show current date
- history — show command history
- echo — print text
- reboot — restart system
- help — show all commands


## Requirements
- NASM
- GCC
- LD
- QEMU
- mtools

## Build

Install requirements:
```bash
sudo apt install nasm gcc binutils qemu-system-x86 mtools

```bash
# Build bootloader
nasm -f bin boot.asm -o boot.bin

# Compile kernel
gcc -m32 -ffreestanding -fno-builtin -fno-pic -fno-pie -nostdlib -c kernel.c -o kernel.o
gcc -m32 -ffreestanding -fno-builtin -fno-pic -fno-pie -nostdlib -c vga.c -o vga.o
gcc -m32 -ffreestanding -fno-builtin -fno-pic -fno-pie -nostdlib -c keyboard.c -o keyboard.o
gcc -m32 -ffreestanding -fno-builtin -fno-pic -fno-pie -nostdlib -c ata.c -o ata.o

# Link kernel
ld -m elf_i386 -T link.ld -o kernel.elf kernel.o vga.o keyboard.o ata.o

# Convert ELF to raw binary
objcopy -O binary kernel.elf kernel.bin

# Combine bootloader and kernel
cat boot.bin kernel.bin > os.bin
```

## Create Disk Image

```bash
# Create floppy image
dd if=/dev/zero of=disk.img bs=512 count=2880

# Format as FAT12
mkfs.fat -F 12 disk.img

# Add test file
echo "Hello" > test.txt
mcopy -i disk.img test.txt ::test.txt

# Write bootloader
dd if=boot.bin of=disk.img bs=446 count=1 conv=notrunc

# Write kernel
dd if=kernel.bin of=disk.img bs=512 seek=100 conv=notrunc
```

## Run

```bash
qemu-system-x86_64 -drive format=raw,file=disk.img
```

## License

Copyright (C) 2026 Gyimah-Benjamin

Licensed under the GNU General Public License v3.0.
