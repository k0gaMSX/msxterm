
#include "tnix/assert.h"
#include "tnix/kernel.h"


int __fail_assert(char * exp, char * file, int line)
{
     printk("Assertion %s:%d: %s\n", file, line, exp);
     panic("Assertion failure");
}

