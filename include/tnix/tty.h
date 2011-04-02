#ifndef _TTY_H_
#define _TTY_H_

#include "arch/types.h"

#define RS232_TTY         0
#define MSX_TTY           1

void tty(void);
void tty_mode(unsigned char mask);
void put_queue(unsigned char scancode);
void puts_queue(unsigned char *str, unsigned count);

#endif /* _TTY_H_ */
