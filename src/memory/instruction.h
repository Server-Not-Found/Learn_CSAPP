#ifndef inst_guard
#define inst_guard

#include<stdlib.h>
#include<stdint.h>

//定义指令集的大小
#define NUM_INSTRUCTIONS 15

// //定义一个模拟内存含有1000个byte
// #define MM_LEN 1000
// uint8_t mm[MM_LEN];

//枚举类型,操作方式
typedef enum OP
{
    mov_reg_reg,    //0
    mov_reg_mem,    //1
    mov_mem_reg,    //2
    push_reg,       //3
    pop_reg,        //4
    call,           //5
    ret,            //6
    add_reg_reg,    //7
} op_t;

//枚举类型
typedef enum OD_TYPE
{
    EMPTY,
    IMM,    //立即数寻址 
    REG,    //寄存器寻址
    MM_IMM, //绝对寻址
    MM_REG,
    MM_IMM_REG,
    MM_REG1_REG2,
    MM_IMM_REG1_REG2,
    MM_REG2_S,
    MM_IMM_REG2_S,
    MM_REG1_REG2_S,
    MM_IMM_REG1_REG2_S
} od_type_t;

//操作数结构体
typedef struct OD
{
    od_type_t type; //寻址方式

    int64_t imm;   //立即数
    //这里的两个寄存器相当于是地址寄存器
    /*以下两个寄存器的指针指向的是一个uint64_t
    是一个虚拟地址，后续通过decode函数实现虚拟地址向物理地址的转换 \
    在物理内存中获取到的数据将被汇编指令（MOV等）进行操作
    */
    uint64_t scale;
    uint64_t *reg1;
    uint64_t *reg2;
} od_t;

//定义指令的格式,相当于手动decode
typedef struct INSTURCT_STRUCT
{
    op_t op;    //mov, push等操作
    //操作数
    od_t src;   //起始位置（可能是立即数，寄存器或者是内存）
    od_t des;
    char code[100]; //字符串注释
} inst_t;

//指令周期函数-
void instruction_cycle();

//不同指令的函数指针声明，函数指针的名称是handler_t
typedef void(*handler_t)(uint64_t,uint64_t);

handler_t handler_table[NUM_INSTRUCTIONS];

//声明译码函数，在instruction.c中实现
uint64_t decode_od(od_t od);

void init_handler_table();

void mov_reg_reg_handler(uint64_t src, uint64_t des);

void add_reg_reg_handler(uint64_t src, uint64_t des);

#endif