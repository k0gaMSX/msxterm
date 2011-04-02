
#ifndef __PRINTK_H_
#define __PRINTK_H_

#ifndef _STDDEF
typedef unsigned size_t;
#endif

extern size_t printk(char * bp, const char *fmt, ...);

#endif /* _PRINTK_H_ */
