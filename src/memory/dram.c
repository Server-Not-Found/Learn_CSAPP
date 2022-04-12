#include<stdio.h>

#include"memory/dram.h"
#include"cpu/mmu.h"
#include"cpu/register.h"

#define SRAM_CACHE_SETTING 0    //是否通过SRAM_CACHE读取,0不读取，1读取

//读取该物理地址中存放的值
uint64_t read64bits_dram(uint64_t paddr)
{
    if(SRAM_CACHE_SETTING == 1)
    {
        return 0x0;
    }

    uint64_t val = 0x0;

    /*
    读取内存的时候需要将uint_8的数据类型准换成uint_64，因为要进行移位操作
    */

    val += (((uint64_t)mm[paddr + 0]) << 0);
    val += (((uint64_t)mm[paddr + 1]) << 8);
    val += (((uint64_t)mm[paddr + 2]) << 16);
    val += (((uint64_t)mm[paddr + 3]) << 24);
    val += (((uint64_t)mm[paddr + 4]) << 32);
    val += (((uint64_t)mm[paddr + 5]) << 40);
    val += (((uint64_t)mm[paddr + 6]) << 48);
    val += (((uint64_t)mm[paddr + 7]) << 56);

    return val;
}

//改写该物理地址存放的值
void write64bits_dram(uint64_t paddr, uint64_t data)
{
    if(SRAM_CACHE_SETTING == 1)
        return;
    
    /*
    将uint64_t(64位8字节)的数据放进单位为uint8_t(8位一字节)的模拟物理内存中
    一个四位的二进制数，比如xxxx，可以表示2^4=16个数，也就可以转换成一个16进制数
    因此0xff相当于一个8位二进制数，也就是表示一个字节
    以下利用0xff和位与运算，实现了取data里面的最低8位数据
    注意：数据的低位会放在内存中靠前的位置
    */
    
    mm[paddr + 0] = (data >> 0) & 0xff;
    mm[paddr + 1] = (data >> 8) & 0xff;
    mm[paddr + 2] = (data >> 16) & 0xff;
    mm[paddr + 3] = (data >> 24) & 0xff;
    mm[paddr + 4] = (data >> 32) & 0xff;
    mm[paddr + 5] = (data >> 40) & 0xff;
    mm[paddr + 6] = (data >> 48) & 0xff;
    mm[paddr + 7] = (data >> 56) & 0xff;
}

//辅助函数
void print_register()
//打印寄存器信息
{
    printf("rax = %16lx\trbx = %16lx\trcx = %16lx\trdx = %16lx\n",
        reg.rax,reg.rbx,reg.rcx,reg.rdx);
    printf("rsi = %16lx\trdi = %16lx\trbp = %16lx\trsp = %16lx\n",
        reg.rsi,reg.rdi,reg.rbp,reg.rsp);
    printf("rip = %16lx\n",reg.rip);

}

void print_stack()
{
    int n = 10;

    /*
    获得rsp存储的虚拟地址对应的物理地址的指针
    rsp寄存器是堆栈指针寄存器，指向栈顶位置
    */
    uint64_t *high = (uint64_t*)&mm[va2pa(reg.rsp)];
    high = &high[n];    //让high跳转到后十位

    uint64_t rsp_start = reg.rsp + 8*n;

    for(int i = 0; i < 2 * n; ++i)
    {
        //从高地址遍历到低地址
        uint64_t *ptr = (uint64_t*)(high-i);
        printf("0x%016lx : %16lx", rsp_start, (uint64_t)*ptr);
    
    if(i == n)
        printf(" <== rsp");
    rsp_start -=8;
    printf("\n");
    }
}

