#include"memory/instruction.h"  //包含指令的各种结构体定义
#include"cpu/mmu.h" //包含虚拟地址转物理地址的函数声明
#include"cpu/register.h"
#include"memory/dram.h"

#include<stdio.h>
#include<stdint.h>

//该函数返回操作数的物理地址
uint64_t decode_od(od_t od)
{
    /*三大类寻址方式:
    1. 立即数寻址，操作数就是一个常数
    2. 寄存器寻址，表示某一个寄存器中存储的值
    3. 内存引用，从内存中获取操作数的值，有很多种地址计算方式
    */
    if (od.type == IMM)
        return *(uint64_t*)&od.imm;
    else if(od.type == REG)
        return (uint64_t)od.reg1; 
    else
    {
        //mm 根据存储器地址计算公式返回地址
        uint64_t vaddr = 0; //定义虚拟地址

        if(od.type == MM_IMM)   //绝对寻址
        {
            vaddr = od.imm;
        }
        else if(od.type == MM_REG)   //间接寻址
        {
            vaddr = *(od.reg1);  //就是将寄存器的值作为操作数的地址值
        }
        else if(od.type == MM_IMM_REG)  //基址+偏移量 寻址
        {
            vaddr = od.imm + *(od.reg1);
        }
        else if(od.type == MM_REG1_REG2)    //变址寻址
        {
            vaddr = *(od.reg1) + *(od.reg2);
        }
        else if(od.type == MM_IMM_REG1_REG2)    //变址寻址
        {
            vaddr = od.imm + *(od.reg1) + *(od.reg2);
        }
        else if(od.type == MM_REG2_S)   //比例变址寻址
        {
            vaddr = (*(od.reg2)) * od.scale;
        }        
        else if(od.type == MM_IMM_REG2_S) //比例变址寻址
        {
            vaddr = od.imm + (*(od.reg2)) * od.scale;
        }        
        else if(od.type == MM_REG1_REG2_S) //比例变址寻址
        {
            vaddr = *(od.reg1) + (*(od.reg2)) * od.scale;
        }        
        else if(od.type == MM_IMM_REG1_REG2_S) //比例变址寻址
        {
            vaddr = od.imm + *(od.reg1) + (*(od.reg2)) * od.scale;
        }      
        
        return va2pa(vaddr);    //最终返回物理地址
    }
}

//定义指令函数
void mov_reg_reg_handler(uint64_t src, uint64_t des)
{
    *(uint64_t *)des = *(uint64_t *)src;
    reg.rip = reg.rip + sizeof(inst_t); //移动到下一个指令
}

void mov_reg_mem_handler(uint64_t src, uint64_t des)
{

}

void mov_mem_reg_handler(uint64_t src, uint64_t des)
{

}

void push_reg_handler(uint64_t src, uint64_t des)
{
    reg.rsp -= 0x8;
    write64bits_dram(va2pa(reg.rsp),*(uint64_t*)src);
    reg.rip += sizeof(inst_t);  //更新程序计数器指针
}

void pop_reg_handler(uint64_t src, uint64_t des)
{
    printf("pop");
}

void call_handler(uint64_t src, uint64_t des)
{
    //src:imm
    //栈顶指针指向低一个64位（八个字节）地址的位置，存放的是返回地址
    reg.rsp -= 0x8;
    // write return address to rsp memory
    // 将return address写到当前rsp指向的位置， call指令执行完之后，回到main函数
    write64bits_dram(va2pa(reg.rsp), reg.rip + sizeof(inst_t));  
    reg.rip = src;
}

void ret_handler(uint64_t src, uint64_t des)
{

}

void add_reg_reg_handler(uint64_t src, uint64_t des)
{
    *(uint64_t *)des = *(uint64_t *)des + *(uint64_t *)src;
    reg.rip = reg.rip + sizeof(inst_t);
}

//初始化函数指针表
void init_handler_table()
{

    handler_table[mov_reg_reg] = &mov_reg_reg_handler;
    handler_table[mov_reg_mem] = &mov_reg_mem_handler;
    handler_table[mov_mem_reg] = &mov_mem_reg_handler;
    handler_table[push_reg] = &push_reg_handler ;
    handler_table[pop_reg] = &pop_reg_handler;
    handler_table[call] = &call_handler;
    handler_table[ret] = &ret_handler;
    handler_table[add_reg_reg] = &add_reg_reg_handler;
}

/*
指令周期函数:
根据以上的decode函数，可以通过内存的物理寻址得到操作数数值
以下函数可以实现CPU取指令和执行指令的作用
*/
void instruction_cycle()
{
    //程序计数器rip指向的指令，从程序计数器处取指令
    inst_t *instr = (inst_t *)reg.rip;

    //获取src和des的内存物理地址
    uint64_t src = decode_od(instr->src); 
    uint64_t des = decode_od(instr->des);
    
    handler_t handler = handler_table[instr->op];   //函数指针，指向不同指令的实现函数
    char * code = instr->code;

    printf("\n*****running instruction: %s *****\n", code);

    //调用指令
    handler(src,des);
    //src和des参数分别是源操作数和目的操作数的物理地址
}