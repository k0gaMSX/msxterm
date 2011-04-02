
#ifndef _STRING_H_
#define _STRING_H_

#include "arch/stddef.h"

extern void *memcpy(void *dest, const  void *src, size_t n);
extern void *memset(void * s, int c, size_t n);
extern size_t strlen(const char *s);
extern char *strcpy(char *dest, const char *orig);
extern char *strcmp(const char s1, const char *s2);

#pragma inline(memcpy)
#pragma inline(memset)
#pragma inline(strlen)
#pragma inline(strcpy)
#pragma inline(strcmp)


#endif /* _STRING_H_ */



