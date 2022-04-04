//memory management unit

#ifndef mmu_guard
#define mmu_guard

#include<stdint.h>

//虚拟地址转物理地址
uint64_t va2pa(uint64_t vaddr);

#endif