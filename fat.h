#ifndef FAT12_H
#define FAT12_H

#define SECTOR_SIZE 512
#define ROOT_DIR_START 19
#define ROOT_DIR_SECTORS 14
#define FAT_START 1
#define DATA_START 33
void cat(char *filename);

struct Dir{
unsigned char name[8];
unsigned char ext[3];
unsigned char attributes;
unsigned char reserved[10];
unsigned short time;
unsigned short data;
unsigned short first_cluster;
unsigned int file_size;
}__attribute__((packed));

#endif
