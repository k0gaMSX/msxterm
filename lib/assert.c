
#include "tnix/assert.h"
#include "tnix/printk.h"


int __fail_assert(char * exp, char * file, int line)
{
     printk("Assertion failed: %s: %s: %s\n", exp, file, line);
}

