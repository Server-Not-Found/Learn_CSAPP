#ifndef DEBUG_GUARD
#define DEBUG_GUARD

#include<stdint.h>

uint64_t string2uint(const char *str);
uint64_t string2uint_range(const char *str, int start, int end);

#endif