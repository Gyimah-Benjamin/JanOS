#include "gdt.h"

struct Gdt_entry gdt[3];
struct Gdt_pointer gdt_ptr;

void gdt_set_entry(int i, unsigned int base, unsigned int limit, 
                   unsigned char access, unsigned char gran){
    gdt[i].base_low    = (base & 0xFFFF);
    gdt[i].base_mid    = (base >> 16) & 0xFF;
    gdt[i].base_high   = (base >> 24) & 0xFF;
    gdt[i].limit_low   = (limit & 0xFFFF);
    gdt[i].granular = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[i].access      = access;
}

void init_gdt(){
    gdt_ptr.limit = (sizeof(struct Gdt_entry) * 3) - 1;
    gdt_ptr.base  = (unsigned int)&gdt;

    gdt_set_entry(0, 0, 0,          0,    0);         // null
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);      // code
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);      // data

    __asm__ volatile("lgdt %0" :: "m"(gdt_ptr));

    // reload segment registers
    __asm__ volatile(
        "mov $0x10, %ax\n"
        "mov %ax, %ds\n"
        "mov %ax, %es\n"
        "mov %ax, %fs\n"
        "mov %ax, %gs\n"
        "mov %ax, %ss\n"
        "ljmp $0x08, $1f\n"
        "1:\n"
    );
}
