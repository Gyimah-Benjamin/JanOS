#ifndef ATA_H
#define ATA_H

void ata_wait();
void ata_ready();

int ata_write(int lba, char *buffer);
int ata_read(int lba, char *buffer);

#endif
