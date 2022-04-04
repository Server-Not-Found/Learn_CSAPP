#ifndef dram_guard
#define dram_guard

#include<stdint.h>

#define MM_LEN 1000 //设定模拟的物理内存长度位1000个byte

uint8_t mm[MM_LEN]; //物理内存

uint64_t read64bits_dram(uint64_t paddr);
void write64bits_dram(uint64_t paddr, uint64_t data);

void print_register();
void print_stack();

#endif