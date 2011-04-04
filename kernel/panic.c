
/*
 * This function is called when a
 */


#include "tnix/kernel.h"

void panic(const char *s)
{
     printk("Kernel panic: %s\n", s);
     for(;;)
          /* nothing */ ;
}
