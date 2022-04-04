#ifndef reg_guard
#define reg_guard

#include<stdlib.h>
#include<stdint.h>

//定义CPU寄存器结构
typedef struct REG_STRUCT
{
    //都是一些寄存器
    //共享低地址
    union
    {
        struct 
        {
            uint8_t al; //低地址
            uint8_t ah; //高地址
        };
        uint16_t ax;
        uint32_t eax;
        uint64_t rax;
    };
    //以下的寄存器也是以上的操作
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t rip;   //程序计数器
} reg_t;

reg_t reg;

#endif