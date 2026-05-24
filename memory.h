#ifndef MEMORY_H
#define MEMORY_H

#define SIZE 4096
#define MEMORY 1048576
#define PAGE_COUNT  (MEMORY/SIZE)
#define HEAP_SIZE 1024

char bitmap[PAGE_COUNT/8] __attribute__((section(".bss")));
char heap[HEAP_SIZE] __attribute__((section(".bss")));

__attribute__((aligned(4096), section(".bss"))) unsigned int page_directory[1024];
__attribute__((aligned(4096), section(".bss"))) unsigned int page_table[1024];


char* heap_pointer = heap;

struct data{
int siz;
int free;
};


void set_free(int page_num){
int byte = page_num/8;
int bit = page_num%8;
bitmap[byte] = bitmap[byte] & ~(1<<bit);

}

void set_taken(int page_num){
int byte = page_num/8;
int bit = page_num%8;
bitmap[byte] = bitmap[byte] | (1<<bit);
}


int find_free(){
for(int i = 0; i <= PAGE_COUNT; i++){
int byte = i/8;
int bit = i%8;
if(!(bitmap[byte] & (1<<bit))){
return i;
}
}
return -1;
}

void set_page_table(){
for(int i = 0; i < 1024; i++){
page_table[i] = (i * SIZE) | 3;
}

page_directory[0] = ((unsigned int)page_table) | 3;

for(int i = 1; i < 1024; i++){
page_directory[i] = 2;
}
}


void paging(){
__asm__ volatile("mov %0, %%cr3" :: "r"((unsigned int)page_directory));

unsigned int cr0;
__asm__ volatile("mov %%cr0, %0" : "=r"(cr0));

cr0 = cr0 | 0x80000000;

__asm__("mov %0, %%cr0" :: "r"(cr0));
}


void *my_malloc(int size){
struct data *position = (struct data*)heap;

while(position < (struct data*)heap_pointer){
if((position -> free) == 1 && (position -> siz) >= size){
position -> free = 0;
return (void*)(position + 1);
}
position = (struct data*)((char*)(position + 1) + position -> siz);
}


struct data *label = (struct data*)heap_pointer;
label -> siz = size;
label -> free = 0;
heap_pointer = heap_pointer + sizeof(struct data) + size;
return (void*)(label + 1);
}

void my_free(void *ptr){
struct data *label = (struct data*)ptr - 1;
label -> free = 1;
}
#endif
