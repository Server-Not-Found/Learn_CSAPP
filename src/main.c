#include<stdio.h>
#include<stdint.h>

#include"cpu/register.h"
#include"cpu/mmu.h"
#include"memory/instruction.h"
#include"memory/dram.h"
#include"disk/elf.h"

int main()
{
    init_handler_table();   //初始化指令集对应的函数指针

    // init register， 初始化寄存器存放的地址
    reg.rax = 0x12340000;
    reg.rbx = 0x0;
    reg.rcx = 0x7ffff7dca758;
    reg.rdx = 0xabcd;
    reg.rsi = 0x7fffffffde38;
    reg.rdi = 0x1;
    reg.rbp = 0x7fffffffdd50;
    reg.rsp = 0x7fffffffdd30;
    reg.rip = (uint64_t)&program[11];   //程序计数器一开始指向了main函数的第一条指令

    //通过封装好的写入函数来写入物理内存
    write64bits_dram(va2pa(0x7fffffffdd50),0x00400590);
    write64bits_dram(va2pa(0x7fffffffdd48),0x0);
    write64bits_dram(va2pa(0x7fffffffdd40),0xabcd);
    write64bits_dram(va2pa(0x7fffffffdd38),0x12340000);
    write64bits_dram(va2pa(0x7fffffffdd30),0x08000660);

    //通过虚拟内存转物理内存的函数获取物理地址
    // uint64_t pa = va2pa(0x7fffffffdd50);

    // printf("%16lx\n",*((uint64_t *)(&mm[pa])));

    print_register();
    print_stack();

    // run instruction
    for(int i=0;i<5;i++)
    {
        instruction_cycle();
        print_register();
        print_stack();
    }
    
    //验证
    int match = 1;

    match = match && (reg.rax == 0x1234abcd);
    match = match && (reg.rbx == 0x0);
    match = match && (reg.rcx == 0x7ffff7dca758);
    match = match && (reg.rdx == 0xabcd);
    match = match && (reg.rsi == 0x7fffffffde38);
    match = match && (reg.rdi == 0x1);
    match = match && (reg.rbp == 0x7fffffffdd50);
    match = match && (reg.rsp == 0x7fffffffdd30);

    if(match == 1)
        printf("register match\n");
    else
        printf("register no match\n");

    match = 1;
    match = match && (read64bits_dram(va2pa(0x7fffffffdd50)) == 0x00400590);
    match = match && (read64bits_dram(va2pa(0x7fffffffdd48)) == 0x0);
    match = match && (read64bits_dram(va2pa(0x7fffffffdd40)) == 0xabcd);
    match = match && (read64bits_dram(va2pa(0x7fffffffdd38)) == 0x12340000);
    match = match && (read64bits_dram(va2pa(0x7fffffffdd30)) == 0x08000660);

    if(match == 1)
        printf("memory match\n");
    else
        printf("memory not match\n");

    return 0;
}
