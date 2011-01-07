#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stddef.h>
#include "types.h"

void memsetw(u16 * start, size_t count, u16 value);

#define memmovew(dst, src, size)  memmove(dst, src, size<<1)
#define memcpyw(dst, src, size)  memcpy(dst, src, size<<1)



#endif /* _MEMORY_H_ */


