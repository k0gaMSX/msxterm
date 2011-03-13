#include <stdio.h>
#include <string.h>

void bmove(void * dst, void * src, unsigned size)
{
  memmove(dst, src, size);
}


int fputs(const char *str, FILE *fp)
{
 size_t size = strlen(str);

 return (fwrite(str, size, 1, fp) == size) ? 1 : EOF;
}
