#include "idt.h"
#include "keyboard.h"
#include "timer.h"
struct Idt_address idt[256];
struct Idt_point pointer;

void idt_fill_address(unsigned int pos, unsigned int address, unsigned short code_seg,unsigned char flag){
idt[pos].low_bytes = (address & 0xFFFF);
idt[pos].high_bytes = (address >> 16) & 0xFFFF;
idt[pos].selector = code_seg;
idt[pos].flags = flag;
idt[pos].zero = 0;
}

void pic_initialisation(){
port_send(0x20, 0x11);//tell master PIC to be ready for data
port_send(0xA0, 0x11);//tell slave PIC to be ready for data

port_send(0x21, 0x20);//master PIC start at 32
port_send(0xA1, 0x28);//slave PIC start at 40

port_send(0x21, 0x04);
port_send(0xA1, 0x02);

port_send(0x21, 0x01);
port_send(0xA1, 0x01);

port_send(0x21, 0x00);
port_send(0xA1, 0x00);
}

void initialize_pic(){
pointer.size = sizeof(idt) - 1;
pointer.location = (unsigned int)&idt;
extern void dum_asm();
for(int i = 0; i < 256; i++){
idt_fill_address(i, (unsigned int)dum_asm, 0x08, 0x8E);
}

pic_initialisation();

extern void keyboard_asm();
extern void timer_asm();
idt_fill_address(33, (unsigned int)keyboard_asm, 0x08, 0x8E);
idt_fill_address(32, (unsigned int)timer_asm, 0x08, 0x8E);
__asm__ volatile("lidt %0" :: "m"(pointer));
__asm__ volatile("sti");
}
