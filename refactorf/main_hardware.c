#include<stdio.h>
#include<string.h>
#include<headers/cpu.h>
#include<headers/memory.h>
#include<headers/common.h>

static void TestString2Uint();

int main()
{
    TestString2Uint();
    return 0;
}

static void TestString2Uint()
{
    const char*nums[12] = {
        "0",
        "-0",
        "0x0",
        "1234",
        "0x1234",
        "0xabcd",
        "-0xabcd",
        "-1234",
        "21473124",
        "-1223451",
        "0x800000000",
        "0xffffffffffffffff"
    };
    for(int i=0;i<12;i++)
    {
        printf("%s => %ld\n",nums[i], string2uint(nums[i]));
    }
}