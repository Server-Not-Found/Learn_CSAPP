#include<string.h>
#include<stdio.h>
#include<stdlib.h>
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

    uint64_t uv = 0;    //unsigned value 字符串表示的数值
    int sign_bit = 0;   //0 - positive 正数; 1 - negativae 负数

    //DFA: deterministic finite automata to scan string and get value
    //有限状态自动机
    int state = 0;

    for(int i=start;i<=end;i++)
    {
        char c = str[i];
        if(state==0){
            if(c == '0'){
                state = 1;
                uv = 0;
                continue;
            }
            else if('1' <= c && c <= '9'){
                state = 2;
                uv = c - '0';
                continue;
            }
            else if(c == '-'){
                state = 3;
                sign_bit = 1;
                continue;
            }
            else if(c == ' '){
                state = 0;
                continue;
            }
            else{
                goto fail;
            }
        }
        else if(state == 1){
            if('0' <= c && c <= '9'){
                state = 2;
                uv = uv * 10 + c - '0';
                continue;
            }
            else if(c == 'x'){
                state = 4;
                continue;
            }
            else if(c == ' '){
                state = 6;
                continue;
            }
            else{ goto fail; }
        } 
        else if(state == 2){
            if('0' <=  c && c <= '9'){
                state = 2;
                uint64_t pv = uv;   //previous value
                uv = uv * 10 + c - '0';
                //有可能发生unsigned overflow
                if(pv > uv)
                {
                    printf("(uint64_t)%s overflow: cannot convert\n",str);
                    goto fail;
                }
                continue;
            }
            else if(c == ' '){
                state = 6;
                continue;
            }
        }
        else if(state == 3){
            if(c == '0'){
                // printf("fuck you!!!");
                state = 1;
            }
            else if('1' <= c && c <= '9'){
                state = 2;
                uv = c - '0';
                continue;
            }
        }
        else if(state == 4){
            if('0' <= c && c <= '9'){
                state = 5;
                uv = uv * 16 + c - '0';
                continue; 
            }
            else if('a' <= c && c <= 'f'){
                state = 5;
                uv = uv * 16 + c - 'a' + 10;
                continue;  
            }
            else {goto fail;}
        }
        else if(state == 5){
            if('0' <= c && c <= '9'){
                state = 5;
                uint64_t pv = uv;
                uv = uv * 16 + c - '0';
                //处理unsigned overflow
                if(pv > uv){
                    printf("(uint64_t)%s overflow: cannot convert\n",str);
                    goto fail;
                }
                continue;
            }
            else if('a' <= c && c <= 'f'){
                state = 5;
                uv = uv * 16 + c - 'a' + 10;
                continue;  
            }
            else {goto fail;}
        }
        else if(state == 6){
            if(c == ' '){
                state = 6;
                continue;
            }
            else {goto fail;}
        }
    }

    if(sign_bit == 0)   return uv;  //正数
    else if(sign_bit == 1)
    {
        int64_t sv = -1 * (int64_t)uv;
        return *(uint64_t *)sv;
    }

    return 0;

    fail:
        printf("type convert: <%s> cannot convert to integer\n",str);
        exit(0);
}