#ifndef _KBD_H_
#define _KBD_H_

#include "arch/types.h"
#define KBD_REPEAT     0x01
#define KBD_CKMODE     0x02
#define KBD_APPLIC     0x04
#define KBD_CRLF       0x08
#define KBD_RAW        0x10

unsigned char is_kbd(unsigned char test);
void kbd_rstmode(unsigned char mask);
void kbd_setmode(unsigned char mask);

#endif /* _KBD_H_ */
