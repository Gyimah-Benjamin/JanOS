#include "keyboard.h"

char last_key = 0;
int shift = 0;

char low[] = {
        0, 0, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
        0,
        'q','w','e','r','t','y','u','i','o','p','[',']',
        '\n', 0,
        'a','s','d','f','g','h','j','k','l',';','\'','`',
        0, '\\',
        'z','x','c','v','b','n','m',',','.','/',
        0, 0, 0,
        ' '
    };


char up[] = {
          0,
          27,
         '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
         '_', '+',
         '\b','\t',

         'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
         '{', '}','\n',0,

         'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
         ':', '"', '~',0,'|',

        'Z', 'X', 'C', 'V', 'B', 'N', 'M',
        '<', '>', '?',

        0,

        '*',

        0,

        ' ',

             };

void port_send_2_bytes(unsigned short port, unsigned short value){
__asm__("out %%ax, %%dx" :: "a"(value), "d"(port));
}

unsigned short port_read_2_bytes(unsigned short port){
unsigned short result;
__asm__("in %%dx, %%ax" : "=a"(result) : "d"(port));
return result;
}

unsigned char port_read(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

void port_send(unsigned short port, unsigned short value) {
    __asm__("out %%al, %%dx" :: "a"(value), "d"(port));
}

void get_key() {
    unsigned char scancode;

    scancode = port_read(0x60);
    if(scancode == 0x2A || scancode == 0x36){
shift = 1;
return;
}

if(scancode == 0xAA || scancode == 0xB6){
shift = 0;
return;
}

if(scancode == 0xE0){
while(!(port_read(0x64) & 0x01));
unsigned char scancode2 = port_read(0x60);
if(scancode2 == 0x48){
last_key = UP;
return;
}
else if(scancode2 == 0x50){
last_key = DOWN;
return;
}
}
    if (scancode & 0x80) return;

    if (scancode < sizeof(low)){
        last_key = shift?up[scancode]:low[scancode];
}
    return;
}
