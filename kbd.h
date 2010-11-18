#ifndef _KBD_H_
#define _KBD_H_

#include "types.h"
#define KBD_REPEAT     1
#define KBD_CKMODE     2
#define KBD_APPLIC     3
#define KBD_CRLF       4
#define KBD_RS232      5

u8 is_kbd(u8 test);

#endif /* _KBD_H_ */
