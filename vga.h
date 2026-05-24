#ifndef VGA_H
#define VGA_H
#define VIDEO_MEMORY 0xB8000

extern int cursor;

void scroll();
void cur();
void print_char(char c, char color);
void print(const char *str);
void printf(const char *str);
void printR(const char *str);
void printY(const char *str);
void newline();
void clear_screen();

#endif
