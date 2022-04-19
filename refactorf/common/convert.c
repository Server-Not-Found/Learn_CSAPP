#include<string.h>
#include<headers/common.h>

uint64_t string2uint(const char *str)
{
    return string2uint_range(str,0,-1);
}

uint64_t string2uint_range(const char *str, int start, int end)
{
    /*
        start: 闭区间的开始索引
        end:闭区间的结束索引
    */

    //处理end==-1的情况
    end = (end == -1) ? strlen(str)-1 : end;

    uint64_t uv = 0;
    int sign_bit = 0;   //0 - positive ; 1 - negativae

    //DFA: deterministic finite automata to scan string and get value
    int state = 0;
    

    return 0;
}