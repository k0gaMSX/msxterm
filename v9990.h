
#ifndef V9990_H_
#define V9990_H_
#include "types.h"


extern void setpal(const u8 * pal);
extern void vdp_init(void);
extern void setspd(void);
extern void resetspd(void);
extern set_cursor_pos(unsigned x, unsigned y);


#endif /* V9990_H_ */
