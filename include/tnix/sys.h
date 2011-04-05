#ifndef _SYS_H_
#define _SYS_H_

extern void sys_init(void);

typedef unsigned char (*int_handler)(void *data);

void request_irq(int_handler handler, void *data);

#endif /* _SYS_H_ */
