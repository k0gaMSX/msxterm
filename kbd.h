#ifndef _KBD_H_
#define _KBD_H_

#include "types.h"
#define KBD_REPEAT     0x01
#define KBD_CKMODE     0x02
#define KBD_APPLIC     0x04
#define KBD_CRLF       0x08
#define KBD_RS232      0x10
#define KBD_RAW        0x20

u8 is_kbd(u8 test);
void kbd_rstmode(u8 mask);
void kbd_setmode(u8 mask);

#endif /* _KBD_H_ */
