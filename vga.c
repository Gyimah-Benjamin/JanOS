#include "vga.h"

#define NO   0x0F
#define BLUE 0x01
#define RED  0x04
#define YELLOW 0x0E

int cursor = 0;

//void port_send(unsigned short port, unsigned short value);

void scroll() {
    char *video = (char*)VIDEO_MEMORY;
    for (int row = 1; row < 25; row++) {
        for (int col = 0; col < 80; col++) {
            int dst = ((row-1)*80+col)*2;
            int src = (row*80+col)*2;
            video[dst]   = video[src];
            video[dst+1] = video[src+1];
        }
    }
    for (int col = 0; col < 80; col++) {
        int pos = (24*80+col)*2;
        video[pos]   = ' ';
        video[pos+1] = YELLOW;
    }
    cursor -= 80;
}

//void cur() {
  //  port_send(0x3D4, 0x0F);
    //port_send(0x3D5, cursor & 0xFF);
    //port_send(0x3D4, 0x0E);
    //port_send(0x3D5, (cursor >> 8) & 0xFF);
//}

void print_char(char c, char color) {
    if (cursor >= 25*80) scroll();
    cur();
    char *video = (char*)VIDEO_MEMORY;
    video[cursor*2]   = c;
    video[cursor*2+1] = color;
    cursor++;
}

void print(const char *str) {
    int i = 0;
    while (str[i]) { print_char(str[i], BLUE); i++; }
}

void printf(const char *str) {
    int i = 0;
    while (str[i]) { print_char(str[i], NO); i++; }
}

void printR(const char *str) {
    int i = 0;
    while (str[i]) { print_char(str[i], RED); i++; }
}

void printY(const char *str) {
    int i = 0;
    while (str[i]) { print_char(str[i], YELLOW); i++; }
}

void newline() {
    cursor = cursor + (80 - (cursor % 80));
    if (cursor >= 25*80) scroll();
    cur();
}

void clear_screen() {
    char *video = (char*)VIDEO_MEMORY;
    for (int i = 0; i < 80*25*2; i++) video[i] = 0;
    cursor = 0;
    cur();
}
