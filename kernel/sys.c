
#include "tnix/sys.h"
#include "tnix/vt.h"
#include "tnix/printk.h"

void sys_init(void)
{
     con_init();
     printk("Booting tnix");
}
