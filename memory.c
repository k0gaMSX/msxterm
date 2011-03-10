#include "memory.h"


void memsetw(void * start, size_t count, int16_t value)
{
  uint16_t *aux = start;

  while (count--)
    *aux++ = value;
}

