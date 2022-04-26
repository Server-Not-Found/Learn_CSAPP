#ifndef INSTRUCTION_GUARD
#define INSTRUCTION_GUARD

#include <stdint.h>

/*======================================*/
/*      instruction set architecture    */
/*======================================*/

typedef enum OPERAND_TYPE
{
    EMPTY,                  // 0
    IMM,                    // 1
    REG,                    // 2
    MEM,                    // 3
} od_type_t;

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



#define MAX_NUM_INSTRUCTION_CYCLE 100

#endif