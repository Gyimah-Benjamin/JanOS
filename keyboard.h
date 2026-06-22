#ifndef KEYBOARD_H
#define KEYBOARD_H
#define UP 1
#define DOWN 2
extern  char last_key;
extern int shift;
extern char low[];

unsigned char port_read(unsigned short port);
void port_send(unsigned short port, unsigned short value);

void port_send_2_bytes(unsigned short port, unsigned short value);
unsigned short port_read_2_bytes(unsigned short port);

void get_key();

#endif
