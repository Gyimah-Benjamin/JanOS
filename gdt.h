#ifndef GDT_H
#define GDT_H

struct Gdt_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_mid;
    unsigned char access;
    unsigned char granular;
    unsigned char base_high;
} __attribute__((packed));

struct Gdt_pointer {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

void init_gdt();

#endif
