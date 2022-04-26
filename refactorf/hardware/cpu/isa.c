#include<string.h>
#include<headers/instruction.h>
//传递操作数的函数
void parse_operand(const char *str, od_t *od)
{
    /*
        str: 汇编指令的字符串
        od: 存储操作数的结构体指针
    */    
   od->type = EMPTY;
   od->imm = 0;
   od->scale = 0;
   od->reg1 = 0;
   od->reg2 = 0;

   int str_len = strlen(str);

   if(str_len == 0)
   {
       //空的指令
       return;
   }

    if(str[0] == '$')
    {
        //判断为立即数imm
        od->type = IMM;

    }
    else if(str[0] == '%')
    {
        //判断为寄存器
        od->type = REG;
    }
    else
    {
        //判断为内存访问
    }
}