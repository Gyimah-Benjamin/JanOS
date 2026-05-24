#include "ata.h"
#include "keyboard.h"

void ata_wait(){
//wait while disk is busy , bit 7 = 1
while(port_read(0x1F7) & 0x80);
}

void ata_ready(){
//wait until disk is ready to receive data, bit 3 = 1
while(!(port_read(0x1F7) & 0x08));
}

int ata_write(int lba, char *buffer){
ata_wait();

port_send(0x1F6, 0xE0 | ((lba >> 24) & 0x0F)); //drive 0, LBA mode, top 4 bits
port_send(0x1F2, 1); //write 1 sector
port_send(0x1F3, lba & 0xFF);
port_send(0x1F4, (lba >> 8) & 0xFF);
port_send(0x1F5, (lba >> 16) & 0xFF);
port_send(0x1F7, 0x30); // write command

ata_ready();

for(int i = 0; i < 256; i++){
unsigned short data = (unsigned char)buffer[i*2] | ((unsigned char)buffer[i*2+1] << 8);
port_send_2_bytes(0x1F0, data);
}
ata_wait();
return 0;
}

int ata_read(int lba, char *buffer){
ata_wait();

port_send(0x1F6, 0xE0 | ((lba >> 24) & 0x0F)); //drive 0, LBA mode, top 4 bits
port_send(0x1F2, 1); //write 1 sector
port_send(0x1F3, lba & 0xFF);
port_send(0x1F4, (lba >> 8) & 0xFF);
port_send(0x1F5, (lba >> 16) & 0xFF);
port_send(0x1F7, 0x20); // read command

ata_ready();

for(int i = 0; i < 256; i++){
unsigned short data = port_read_2_bytes(0x1F0);
buffer[i*2] = data & 0xFF;
buffer[i*2+1] = (data >> 8) & 0xFF;
}
return 0;
}
