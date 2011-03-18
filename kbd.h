#ifndef _KBD_H_
#define _KBD_H_

#include "types.h"
#define KBD_REPEAT     0x01
#define KBD_CKMODE     0x02
#define KBD_APPLIC     0x04
#define KBD_CRLF       0x08
#define KBD_RS232      0x10
#define KBD_RAW        0x20

int8_t is_kbd(int8_t test);
void kbd_reset(void);
void kbd_rstmode(uint8_t mask);
void kbd_setmode(uint8_t mask);
void scan_matrix(void);

#endif /* _KBD_H_ */
