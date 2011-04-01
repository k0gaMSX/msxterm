
#include "arch.h"
#include "sys.h"


void main(void)                 /* This is not an error */
{                               /* because we are in non hosted */
     arch_init();
     sys_init();

loop:
     ei();
     goto loop;
}
