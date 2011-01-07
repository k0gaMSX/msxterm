#include "memory.h"


void memsetw(u16 * start, size_t count, u16 value)
{
  while (count--)
    *start++ = value;
}

