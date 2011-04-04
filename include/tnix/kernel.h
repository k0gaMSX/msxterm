
#ifndef __KERNEL_H_
#define __KERNEL_H_

/*
 * This header contains some very used routines.
 */

#include "arch/stddef.h"


extern size_t printk(const char *fmt, ...);
extern void panic(const char *s);

#endif /* _KERNEL_H_ */
