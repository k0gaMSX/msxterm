
#include "arch/arch.h"
#include "tnix/sys.h"


void main(void)                 /* This is not an error because we are in */
{                               /* non hosted environment*/
     di();
     arch_init();
     sys_init();

loop:
     ei();
     goto loop;
}
