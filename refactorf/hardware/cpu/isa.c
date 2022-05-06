#include<string.h>
#include<headers/instruction.h>
#include<headers/cpu.h>
#include<headers/common.h>

/*
指令集体系
*/
static const char *reg_name_list[72] = {
    "%rax","%eax","%ax","%ah","%al",
    "%rbx","%ebx","%bx","%bh","%bl",
    "%rcx","%ecx","%cx","%ch","%acl",
    "%rdx","%edx","%dx","%dh","%dl",
    "%rsi","%esi","%si","%sil","%sih",
    "%rdi","%edi","%di","%dil","%dih",
    "%rbp","%ebp","%bp","%bpl","%bph",
    "%rsp","%esp","%sp","%spl","%sph",
    "%r8","%r8d","%r8w","%r8b",
    "%r9","%r9d","%r9w","%r9b",
    "%r10","%r10d","%r10w","%r10b",
    "%r11","%r11d","%r11w","%r11b",
    "%r12","%r12d","%r12w","%r12b",
    "%r13","%r13d","%r13w","%r13b",
    "%r14","%r14d","%r14w","%r14b",
    "%r15","%r15d","%r15w","%r15b",
};

static uint64_t reflect_register(const char *str, core_t *cr)
{
    /*
        str:寄存器名称
        cr:使用的cpu core
    */
    // lookup table
    cpu_reg_t *reg = &(cr->reg);
    uint64_t reg_addr[72] = {
        (uint64_t)&(reg->rax),(uint64_t)&(reg->eax),(uint64_t)&(reg->ax),(uint64_t)&(reg->ah),(uint64_t)&(reg->al),
        (uint64_t)&(reg->rbx),(uint64_t)&(reg->ebx),(uint64_t)&(reg->bx),(uint64_t)&(reg->bh),(uint64_t)&(reg->bl),
        (uint64_t)&(reg->rcx),(uint64_t)&(reg->ecx),(uint64_t)&(reg->cx),(uint64_t)&(reg->ch),(uint64_t)&(reg->cl),
        (uint64_t)&(reg->rdx),(uint64_t)&(reg->edx),(uint64_t)&(reg->dx),(uint64_t)&(reg->dh),(uint64_t)&(reg->dl),
        (uint64_t)&(reg->rsi),(uint64_t)&(reg->esi),(uint64_t)&(reg->si),(uint64_t)&(reg->sih),(uint64_t)&(reg->sil),
        (uint64_t)&(reg->rdi),(uint64_t)&(reg->edi),(uint64_t)&(reg->di),(uint64_t)&(reg->dih),(uint64_t)&(reg->dil),
        (uint64_t)&(reg->rbp),(uint64_t)&(reg->ebp),(uint64_t)&(reg->bp),(uint64_t)&(reg->bph),(uint64_t)&(reg->bpl),
        (uint64_t)&(reg->rsp),(uint64_t)&(reg->esp),(uint64_t)&(reg->sp),(uint64_t)&(reg->sph),(uint64_t)&(reg->spl),
        (uint64_t)&(reg->r8),(uint64_t)&(reg->r8d),(uint64_t)&(reg->r8w),(uint64_t)&(reg->r8b),
        (uint64_t)&(reg->r9),(uint64_t)&(reg->r9d),(uint64_t)&(reg->r9w),(uint64_t)&(reg->r9b),
        (uint64_t)&(reg->r10),(uint64_t)&(reg->r10d),(uint64_t)&(reg->r10w),(uint64_t)&(reg->r10b),
        (uint64_t)&(reg->r11),(uint64_t)&(reg->r11d),(uint64_t)&(reg->r11w),(uint64_t)&(reg->r11b),
        (uint64_t)&(reg->r12),(uint64_t)&(reg->r12d),(uint64_t)&(reg->r12w),(uint64_t)&(reg->r12b),
        (uint64_t)&(reg->r13),(uint64_t)&(reg->r13d),(uint64_t)&(reg->r13w),(uint64_t)&(reg->r13b),
        (uint64_t)&(reg->r14),(uint64_t)&(reg->r14d),(uint64_t)&(reg->r14w),(uint64_t)&(reg->r14b),
        (uint64_t)&(reg->r15),(uint64_t)&(reg->r15d),(uint64_t)&(reg->r15w),(uint64_t)&(reg->r15b),
    };

   //一共有72个寄存器
   for(int i=0;i<72;i++)
   {
       // 匹配寄存器
       if(strcmp(str,reg_name_list[i]) == 0)
       {    
           return reg_addr[i];  //返回寄存器的地址
       }
   }
   printf("parse register %s error\n",str);
   exit(0);
}

void parse_operand(const char *str, od_t *od,core_t *cr)
{
    /*
        将一个操作数映射为一个od_t对象
        str: 汇编指令的字符串
        od: 存储操作数的结构体指针
        cr
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
        od->reg1 = reflect_register(str,cr);
        return;
    }
    else
    {
        //判断为内存访问
        char imm[64] = {'\0'};
        int imm_len = 0;
        char reg1[64] = {'\0'};
        int reg1_len = 0;
        char reg2[64] = {'\0'};
        int reg2_len = 0;
        char scal[64] = {'\0'};
        int scal_len = 0;

        int ca = 0; //括号计数
        int cb = 0; //逗号计数

        for(int i=0;i<str_len;i++)
        {
            char c = str[i];
            
            if(c == '(' || c == ')')
            {
                ca++;
                continue;
            }
            else if(c == ',')
            {
                cb++;
                continue;
            }
            else    //除了逗号和括号的情况，处理当前状态
            {
                if(ca == 0)
                {
                     //xxx
                     imm[imm_len] = c;
                     imm_len++;
                     continue;
                }
                else if(ca == 1)
                {
                    if(cb == 0)
                    {
                        // ???(xxx
                        // (xxx
                        reg1[reg1_len] = c;
                        reg1_len++;
                        continue;
                    }
                    else if(cb == 1)
                    {
                        // ???(???,xxx
                        // (???,xxx
                        // (,xxx
                        // ???(,xxx
                        reg2[reg2_len] = c;
                        reg2_len++;
                        continue;
                    }
                    else if(cb == 2)
                    {
                        // ???(???,???,xxx
                        // (???,???,xxx
                        scal[scal_len] = c;
                        scal_len++;
                        continue;
                    }
                }
            }
        }

        //已经分别获得了imm,reg1,reg2,scal的内容及长度了
        if(imm_len > 0)
        {
            od->imm = string2uint(imm);
            if(ca == 0)
            {
                // imm类型
                od->type = MEM_IMM;
                return;
            }
        }
    }
}