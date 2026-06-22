#include "win.h"
#include "font.h"
void draw_desktop();
void draw_task_bar();
void logo();
struct window_size point_maxim;

void init_screen() {
    screen = (unsigned char*)(*(unsigned int*)0x7E28);
}

void fill_screen(int x, int y, unsigned int color){
    unsigned char *fb = (unsigned char*)screen;
    int offset = (x * 3072) + (y * 3);
    fb[offset]     = color & 0xFF;
    fb[offset + 1] = (color >> 8) & 0xFF;
    fb[offset + 2] = (color >> 16) & 0xFF;
}

void draw_rect(int x, int y, int w, int h, int color){
for(int i = x; i < x + h; i++){
for(int j = y; j < y + w; j++){
fill_screen(i, j, color);
}
}
}

void cur() {
   if(point_maxim.maxim_flag == 1){
    draw_rect(track_x - 2, track_y, 1, 14, RED);
}
}

void cur_clear() {
   if(point_maxim.maxim_flag == 1){
    draw_rect(track_x - 2, track_y, 1, 14, TEXT_B);
}
}

void cur_clear_b() {
   if(point_maxim.maxim_flag == 1){
    draw_rect(track_x - 2, track_y + 10, 1, 14, TEXT_B);
}
}

void term_newline(){
ad_ten += 14;
track_x += 14;
string(67 + ad_ten, 120 + 5, "~", TASK_BG, TEXT_B);
string(65 + ad_ten, 120 + 5 + 10, "$", TASK_BG, TEXT_B);
track_y = 120 + 5 + 10 + 10;
track_y1 = 120 + 5 + 10 + 10;
letter_track = 145;
}

void term_newline_xpromt(){
ad_ten += 14;
track_x += 14;
track_y = 145;
track_y1 += 10;
}

void term_newline_max(){
ad_ten += 14;
track_x += 14;
string(37 + ad_ten, 5, "~", TASK_BG, TEXT_B);
string(35 + ad_ten, 5 + 10, "$", TASK_BG, TEXT_B);
track_y = 5 + 10 + 10;
track_y1 = 5 + 10 + 10;
letter_track = 25;
}

void draw_char(int x, int y, char al, int color, int color1){
unsigned char* conect = (unsigned char*)font[al];
for(int row = 0; row < 8; row++){
unsigned char conect1 = conect[row];
for(int bit = 0; bit < 8; bit++){
if((0x80 >> bit) & conect1){
fill_screen(x + row, y + bit, color);
}
else{
fill_screen(x + row, y + bit, color1);
}
}
}
}

void draw_char_l(int x, int y, char al, int color, int color1, int scale){
unsigned char* conect = (unsigned char*)font[al];
for(int row = 0; row < 8; row++){
unsigned char conect1 = conect[row];
for(int bit = 0; bit < 8; bit++){
if((0x80 >> bit) & conect1){
draw_rect(x + row * scale, y + bit * scale, scale, scale, color);
}

else{
draw_rect(x + row * scale, y + bit * scale, scale, scale, color1);
}
}
}
}

void string_l(int x, int y, char *str, int color, int color1, int scale){
int i = 0;
while(str[i] != 0){
draw_char_l(x, y, str[i], color, color1, scale);
y += 10 * scale;
i++;
}
}

void string(int x, int y, char *str, int color, int color1){
int i = 0;
while(str[i] != 0){
draw_char(x, y, str[i], color, color1);
y += 10;
track_y1 += 10;
i++;
}
}

void terminal(int x, int y, int term_y, int term_x, int term_title, char *str ){
int incre = 0;
draw_rect(x, y, term_y, term_x, TEXT_B);

draw_rect(x, y, term_y, term_title, GREYY);
for(int i = 0; str[i] != 0; i++){
draw_char((term_title + x) /2 + 15, (term_y + y) /2 + incre, str[i], TEXT_W, GREYY);
incre += 10;
}
string(term_title + 37, y + 5, "~", TASK_BG, TEXT_B);
string(term_title + 35, y + 5 + 10, "$", TASK_BG, TEXT_B);

}

void terminal_max(int x, int y, int term_y, int term_x, int term_title, char *str ){
int incre = 0;
draw_rect(x, y, term_y, term_x, TEXT_B);

draw_rect(x, y, term_y, term_title, GREYY);
for(int i = 0; str[i] != 0; i++){
draw_char((term_title - 30) + 13, (term_y / 2 - 45) + incre, str[i], TEXT_W, GREYY);
incre += 10;
}
for(int i = 0; i < 728; i++){
for(int x = 0; x < 1024; x++){
for(int y = 0; y < 3; y++){
}
}
}
}

void terminal_close(int x, int y, int term_y, int term_x, int term_title, char *str){
int incre = 0;
draw_rect(x, y, term_y, term_x, DESK_BG);

draw_rect(x, y, term_y, term_title, DESK_BG);
for(int i = 0; str[i] != 0; i++){
draw_char((term_title + x) /2 + 15, (term_y + y) /2 + incre, str[i], DESK_BG, DESK_BG);
incre += 10;
}
string(term_title + 37, y + 5, "~", DESK_BG, DESK_BG);
string(term_title + 35, y + 5 + 10, "$", DESK_BG, DESK_BG);

}


void delay(unsigned int time){
for(int i = 0; i < time; i++);
}

void draw_cursor(int x, int y, int visible){
if(visible){
draw_rect(x-7, y, 2, 27, RED);
}

else{
draw_rect(x-7, y, 2, 27, TEXT_B);
}

}

void boot_anime(){
draw_rect(0, 0, SCREEN_W, SCREEN_H, 0x00000000);

int lx = SCREEN_W / 2 - 100;
int ly = SCREEN_H / 2 - 40;

char *name = "JanOS";

for(int i = 0; name[i] != 0; i++){
for(int j = 0; j < 1; j++){
draw_cursor(ly, lx, 1);
delay(122005000);
draw_cursor(ly, lx, 0);
delay(122005000);
}

draw_char_l(ly, lx + 8, name[i], 0x00444444, 0x00000000, 0);
delay(90000000);
draw_char_l(ly, lx + 4, name[i], 0x00888888, 0x00000000, 1);
delay(90000000);
draw_char_l(ly, lx, name[i], TEXT_W, 0x00000000, 2);
lx += 10 * 2;

}

for(int i = 0; i < 6; i++){
draw_cursor(ly, lx, i%2);
delay(160000000);
}

draw_cursor(ly, lx, 0);
delay(200000000);

draw_desktop();
draw_task_bar();
logo();
}

void logo(){
int lx = SCREEN_W / 2 - 100;
int ly = SCREEN_H / 2 - 40;

string_l(ly, lx, "JanOS",TEXT_W, DESK_BG, 2);
}

void draw_desktop(){
draw_rect(0, 0, SCREEN_W, SCREEN_H - TASK_H, DESK_BG);
}

void draw_task_bar(){
draw_rect(TASK_Y, 0, SCREEN_W, TASK_H, TASK_BG);
}
