#include "memory.h"
#include "fat.h"
#include "vga.h"
#include "keyboard.h"
#include "ata.h"
#include "idt.h"
#include "timer.h"
#include "gdt.h"
//#include "calcul.h"
#define GREEN 0x02
#define NO 0x0F
#define GREY 0x07
#define YELLOW 0x0E
#define VIOLET 0x05
void reeb();
void shut();
void write_file(char *filename, char *data, int size);
void rm(char *filename);
char history_store[10][255];
extern char last_key;

void init_history(){
for(int i = 0; i < 10; i++){
for(int j = 0; j < 255; j++){
history_store[i][j] = 0;
}
}
}

int strcmp(char* a, char* b){
int count = 0;
while(a[count] && b[count]){
if(a[count] != b[count]){
return 0;
}
count++;
}
return a[count] == b[count];
}

int strlen(char *a){
int i;
for(i = 0; a[i]!= 0; i++);
return i;
}

void read_sector(int size, char* buffer){
char* disk = (char*)0x9000;
char* position = disk + (size * SECTOR_SIZE);
for(int i = 0; i < SECTOR_SIZE; i++){
buffer[i] = position[i];
}
}

void ls(){
char* sector = (char*)my_malloc(512);
struct Dir* level;
for(int s = 0; s < 14; s++){
read_sector(ROOT_DIR_START + s, sector);
level = (struct Dir*)sector;

for(int i = 0; i < SECTOR_SIZE/sizeof(struct Dir); i++){
if((level + i) -> name[0] == 0x00){
my_free(sector);
return;
}
if((level + i) -> name[0] == 0xE5){
continue;
}
if((level + i) -> attributes == 0x0F){
continue;
}

for(int j = 0; j < 8; j++){
if((level + i) -> name[j] == ' '){
break;
}
print_char((level + i) -> name[j], GREEN);
}

if((level + i) -> ext[0] != ' '){
print_char('.', GREEN);
for(int j = 0; j < 3; j++){
if((level + i) -> ext[j] == ' '){
break;
}
print_char((level + i) -> ext[j], GREEN);
}
}
newline();
}
}
my_free(sector);
}

void cat(char *filename) {
    char *sector = (char*)my_malloc(512);
    struct Dir *level;

    // separate filename and extension
    char name[8] = {' ',' ',' ',' ',' ',' ',' ',' '};
    char ext[3]  = {' ',' ',' '};

    int i = 0;
    int j = 0;
    while (filename[i] && filename[i] != '.') {
        name[j++] = filename[i++];
    }
    if (filename[i] == '.') {
        i++;
        j = 0;
        while (filename[i]) {
            ext[j++] = filename[i++];
        }
    }

    for (int s = 0; s < 14; s++) {
        read_sector(ROOT_DIR_START + s, sector);
        level = (struct Dir*)sector;

        for (int i = 0; i < SECTOR_SIZE / sizeof(struct Dir); i++) {
            if ((level+i)->name[0] == 0x00) { my_free(sector); printR("File not found"); return; }
            if ((level+i)->name[0] == 0xE5) continue;
            if ((level+i)->attributes == 0x0F) continue;

            // match name and extension
            int match = 1;
            for (int j = 0; j < 8; j++) {
                if ((level+i)->name[j] != name[j]) { match = 0; break; }
            }
            for (int j = 0; j < 3; j++) {
                if ((level+i)->ext[j] != ext[j]) { match = 0; break; }
            }

            if (match) {
                int cluster = (level+i)->first_cluster;
                int size = (level+i)->file_size;
                int sector_num = DATA_START + (cluster - 2);

                char *file_buf = (char*)my_malloc(size);
                for(int k = 0; k < size; k++){
                file_buf[k] = 0;
                }
                read_sector(sector_num, file_buf);

                newline();
                for(int k = 0; k < size; k++){
                if(file_buf[k] == '\r'){
                continue;
                }
                if(file_buf[k] == '\n'){
                newline();
                continue;
                }
                print_char(file_buf[k], YELLOW);
                }
                newline();

                my_free(file_buf);
                my_free(sector);
                return;
            }
        }
    }
    my_free(sector);
    printR("File not found");
    newline();
}

void print_tim();

void date();

void print_uptime(){
unsigned int seconds = ticks/18;
unsigned int minutes = seconds/60;
unsigned int hours = minutes/60;

seconds = seconds % 60;
minutes = minutes % 60;
printY("Up-time:");
print_char('0' + hours/10, GREEN);
print_char('0' + hours%10, GREEN);

printY(":");

print_char('0' + minutes/10, GREEN);
print_char('0' + minutes%10, GREEN);

printY(":");

print_char('0' + seconds/10, GREEN);
print_char('0' + seconds%10, GREEN);
}
void shell(char* cmd){
if(cmd[0] == 0){
return;
}
if(strcmp(cmd, "help")){
printY("--------------------------Available commands--------------------------");
newline();
printY("----------->>>>>help, about, clear, version, echo, time<<<<<----------");
newline();
printY("-------->>>>>date, reboot, history, ls, cat, write, rm, uptime<<<<<----------");
newline();
}
else if(strcmp(cmd, "about")){
printY("JanOS Corporation.");
newline();
}

else if(strcmp(cmd, "clear")){
clear_screen();
}
else if(strcmp(cmd, "version")){
printY("Version 1.0x");
newline();
}

else if(cmd[0] == 'e' && cmd[1] == 'c' && cmd[2] == 'h' && cmd[3] == 'o' && cmd[4] == ' '){
printY(cmd + 5);
newline();
}

else if(strcmp(cmd, "time")){
print_tim();
newline();
}

else if(strcmp(cmd, "date")){
date();
newline();
}

else if(strcmp(cmd, "reboot")){
reeb();
newline();
}

else if(strcmp(cmd, "shutdown")){
shut();
newline();
}

else if(strcmp(cmd, "uptime")){
print_uptime();
newline();
}

else if(strcmp(cmd, "history")){
int counter = 0;
while(counter < 10){
if(history_store[counter % 10][0] != 0){
for(int i = 0; history_store[counter % 10][i] != 0; i++){
print_char(history_store[counter % 10][i], VIOLET);
}
newline();
}
counter++;
}
}

else if(strcmp(cmd, "ls")){
ls();
}

else if(cmd[0] == 'c' && cmd[1] == 'a' && cmd[2] == 't' && cmd[3] == ' '){
cat(cmd + 4);
newline();
}

else if(cmd[0] == 'w' && cmd[1] == 'r' && cmd[2] == 'i' && cmd[3] == 't' && cmd[4] == 'e'){
if(cmd[5] != ' '){
printR("Guide: write filename file_content");
newline();
}
else{
int k = 6;
while(cmd[k] != 0 && cmd[k] != ' ')k++;
if(cmd[k] == 0){
printR("Guide: write filename file_content");
newline();
}
else{
cmd[k] = 0;
write_file(cmd + 6, cmd + k + 1, strlen(cmd + k + 1));
}
}
}

else if(cmd[0] == 'r' && cmd[1] == 'm' && cmd[2] == ' '){
rm(cmd + 3);
newline();
}
//else if(cmd[0] == 'c' && cmd[1] == 'a' && cmd[2] == 'l' && cmd[3] == 'c' && cmd[4] == ' '){
//calc(cmd + 5,strlen(cmd + 5));
//newline();
//}

else{
printR("Command does not exist yet.");
newline();
}
}

int find_cluster(){
char *sector = (char*)my_malloc(512);
read_sector(FAT_START, sector);
for(int i = 2; i < 29; i++){
int offset = i + (i/2);
unsigned short val;

if(i % 2 == 0){
val = sector[offset] | ((sector[offset + 1]) & 0x0F) << 8;
}

else{
val = (sector[offset]) >> 4 | (sector[offset + 1]) << 4;
}

if(val == 0x000){
my_free(sector);
return i;
}
}
my_free(sector);
return -1;
}

void clus_taken(char *fat, int clust){
int offset = clust + (clust / 2);
if(clust % 2 == 0){
fat[offset] = 0xFF;
fat[offset + 1] = (fat[offset + 1] & 0xF0) | 0x0F;
}

else{
fat[offset] = (fat[offset] & 0x0F) | 0xF0;
fat[offset + 1] = 0xFF;
}

}

void write_file(char *filename, char *data, int size){
char *sector = (char*)my_malloc(512);

char file_name[8] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
char ext[3] = {' ', ' ', ' '};

int i = 0, j = 0;
while(filename[i] && filename[i] != '.' && j < 8){
file_name[j++] = filename[i++];
}

if(filename[i] == '.'){
i++;
j = 0;

while(filename[i] && j < 3){
ext[j++] = filename[i++];
}
}

else{
ext[0] = 'T';
ext[1] = 'X';
ext[2] = 'T';
}

int cluster = find_cluster();
if(cluster == -1){
printR("Disk full");
my_free(sector);
return;
}

char *fat = (char*)my_malloc(512);
read_sector(FAT_START, fat);
clus_taken(fat, cluster);

char *disk = (char*)0x9000;
char *real_fat = disk + (FAT_START * SECTOR_SIZE);
for(int i = 0; i < 512; i++){
real_fat[i] = fat[i];
}
ata_write(FAT_START, real_fat);

my_free(fat);

for(int s = 0; s < 14; s++){
read_sector(ROOT_DIR_START + s, sector);
struct Dir *level = (struct Dir*)sector;

for(int i = 0; i < SECTOR_SIZE/sizeof(struct Dir); i++){

if((level + i) -> name[0] == 0x00){
break;
}
if((level + i) -> name[0] == 0xE5){
continue;
}

int exist = 1;

for(int j = 0; j < 8; j++){
if((level + i) -> name[j] != file_name[j]){
exist = 0;
break;
}
}

if(exist){
printR("File already exist");
newline();
return;
}
}
for(int i = 0; i < SECTOR_SIZE/sizeof(struct Dir); i++){

if((level + i) -> name[0] == 0x00 || (level + i) -> name[0] == 0xE5){
for(int j = 0; j < 8; j++){
(level + i) -> name[j] = file_name[j];
}

for(int j = 0; j < 3; j++){
(level + i) -> ext[j] = ext[j];
}

(level + i) -> attributes = 0x20;
(level + i) -> first_cluster = cluster;
(level + i) -> file_size = size;

char *real_dir = disk + ((ROOT_DIR_START +s) * SECTOR_SIZE);
for(int j = 0; j < 512; j++){
real_dir[j] = sector[j];
}
ata_write(ROOT_DIR_START+s, real_dir);

char *dst = disk + ((DATA_START + cluster -2) * SECTOR_SIZE);
for(int j = 0; j < size; j++){
dst[j] = data[j];
}
ata_write(DATA_START + cluster -2, dst);

my_free(sector);
printY("File written successfully");
newline();
return;
}
}
}
my_free(sector);
printR("Disk is full");
newline();
}

void rm(char *filename){
char name_1[8] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
char ext_1[3] = {' ', ' ', ' '};
int i = 0, j = 0;

while(filename[i] != '\0'){
if(filename[i] == '.'){
i++;
int v = 0;
while(filename[i] && v < 3){
ext_1[v] = filename[i];
i++;
v++;
}
break;
}
if(j < 8){
name_1[j] = filename[i];
j++;
}
i++;
}
char *sector = (char*)my_malloc(512);
struct Dir *level = (struct Dir*)sector;

for(int s = 0; s < 14; s++){
read_sector(ROOT_DIR_START + s, sector);

for(int i = 0; i < SECTOR_SIZE/sizeof(struct Dir); i++){
if((level + i) -> name[0] == 0x00){
break;
}

if((level + i) -> name[0] == 0xE5){
continue;
}

int x, v = 0;
int match = 1;
for(x = 0; x < 8; x++){
if((level + i) -> name[x] != name_1[x]){
match = 0;
break;
}
}
while(v < 3){
if((level + i) -> ext[v] != ext_1[v]){
match = 0;
break;
}
v++;
}

if(match){

(level + i) -> name[0] = 0xE5;
printY("File deleted");
newline();

char *buff = (char*)my_malloc(512);

read_sector(FAT_START, buff);

int cluster = (level + i) -> first_cluster;
int cluster_i = cluster + (cluster/2);

if(cluster%2 == 0){
buff[cluster_i] = 0x00;
buff[cluster_i + 1] = (buff[cluster_i + 1] & 0xF0);
}

else{
buff[cluster_i] = (buff[cluster_i] & 0x0F);
buff[cluster_i + 1] = 0x00;
}

char *disk = (char*)0x9000;
char *fat = disk + (FAT_START * SECTOR_SIZE);
for(int n = 0; n < 512; n++){
fat[n] = buff[n];
}
ata_write(FAT_START, fat);
my_free(buff);

char *dir_T = disk + ((ROOT_DIR_START + s) * SECTOR_SIZE);
for(int m = 0; m < 512; m++){
dir_T[m] = sector[m];
}
ata_write(ROOT_DIR_START + s, dir_T);
my_free(sector);
return;
}

}
}
printR("File does not exist");
my_free(sector);
return;
}

unsigned char rtc(unsigned char reg){
__asm__("out %%al, %%dx" :: "a"(reg), "d"(0x70));

unsigned char val;
__asm__("in %%dx, %%al" : "=a"(val) : "d"(0x71));

return val;
}


void print_tim(){
unsigned char hours = rtc(0x04);
unsigned char minutes = rtc(0x02);
unsigned char seconds = rtc(0x00);

hours = ((hours >> 4)*10 + (hours & 0x0F));
minutes = ((minutes >> 4)*10 + (minutes & 0x0F));
seconds = ((seconds >> 4)*10 + (seconds & 0x0F));

print_char('0' + hours/10, GREEN);
print_char('0' + hours%10, GREEN);

print_char(':', GREEN);

print_char('0' + minutes/10, GREEN);
print_char('0' + minutes%10, GREEN);

print_char(':', GREEN);

print_char('0' + seconds/10, GREEN);
print_char('0' + seconds%10, GREEN);
}

void date(){
unsigned char day = rtc(0x07);
unsigned char month = rtc(0x08);
unsigned char year = rtc(0x09);

day = (day >> 4)*10 + (day & 0x0F);
month = (month >> 4)*10 + (month & 0x0F);
year = (year >> 4)*10 + (year & 0x0F);
print_char('0' + day/10, GREEN);
print_char('0' + day%10, GREEN);

print_char('/', GREEN);

print_char('0' + month/10, GREEN);
print_char('0' + month%10, GREEN);

print_char('/', GREEN);

print_char('2', GREEN);
print_char('0', GREEN);
print_char('0' + year/10, GREEN);
print_char('0' + year%10, GREEN);

}


void reeb(){
port_send(0x64, 0xFE);
}

void shut(){
port_send(0x604, 0x2000);
}
__attribute__((section(".text.kernel_main")))
void kernel_main() {
    init_history();
    clear_screen();
    init_gdt();
    initialize_pic();
    set_page_table();
    paging();
//    clear_screen();
    print("                   ");
    print("Jan Operating System Version 1.0");
    newline();
char buffer[1024];
int history_count = 0;
int browse = 0;
int arrow_count = 0;
int position = 0;
int pro = 0;

print("~$ ");
pro = cursor;
    while (1) {
if(last_key){

if(last_key == '\b' && cursor > pro){
position--;
cursor--;
print_char(' ', NO);
cursor--;
cur();
last_key = 0;
}

else if(last_key == DOWN){
if(arrow_count > 0){
arrow_count--;
}

while(cursor > pro){
cursor--;
char *video = (char*)VIDEO_MEMORY;
video[cursor * 2] = ' ';
video[cursor * 2 + 1] = NO;
}
if(arrow_count == 0){
buffer[0] = 0;
position = 0;
cur();
}
else{
int i;
int browse = (history_count - arrow_count) % 10;
for(i = 0; history_store[browse][i] != 0; i++){
buffer[i] = history_store[browse][i];
print_char(buffer[i], VIOLET);
}
position = i;
}
last_key = 0;
}

else if(last_key == UP){
    if(arrow_count < 10 && arrow_count < history_count){
        arrow_count++;
    }

    // clear current line
    while(cursor > pro){
        cursor--;
        char *video = (char*)VIDEO_MEMORY;
        video[cursor * 2] = ' ';
        video[cursor * 2 + 1] = NO;
    }

    // get the right slot
    int browse = (history_count - arrow_count) % 10;

    // load into buffer and print
    int i = 0;
    while(history_store[browse][i] != 0){
        buffer[i] = history_store[browse][i];
        print_char(buffer[i], VIOLET);
        i++;
    }
    position = i;
    last_key = 0;
}

else if(last_key == '\n'){
buffer[position] = 0;

if(position > 0){
int i;
for(i = 0; buffer[i] != 0; i++){
history_store[history_count % 10][i] = buffer[i];
}

history_store[history_count % 10][i] = 0;

history_count++;
arrow_count = 0;
}
newline();
shell(buffer);
position = 0;
print("~$ ");
pro = cursor;
last_key = 0;
}

else if(last_key != 0){
buffer[position] = last_key;
print_char(last_key, NO);
position++;
}
last_key = 0;
}
}
}
