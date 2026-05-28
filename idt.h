#ifndef IDT_H
#define IDT_H

struct Idt_address{
unsigned short low_bytes;
unsigned short selector;
unsigned char zero;//code segment 0x08
unsigned char flags;
unsigned short high_bytes;
}__attribute__((packed));

struct Idt_point{
unsigned short size;
unsigned int location;
}__attribute__((packed));

void initialize_pic();

#endif
