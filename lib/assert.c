
#include "tnix/assert.h"
#include "tnix/kernel.h"


int __fail_assert(char * exp, char * file, int line)
{
     printk("%s: %s: %s\n", exp, file, line);
     panic("Assertion failure");
}

