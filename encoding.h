#ifndef _ENCODING_H_
#define _ENCODING_H_
#include "types.h"

#define UCS_INVALID 0xfffd
#define ASCII_INVALID 0x1a

struct encoding {
  void (* reset)(void);
  u8 (* decode)(u8 in, u16 * out);
  u8 (* encode)(u16 in, u8 * out);
};




struct unimap_t {
  u8 bit8;
  u16 bit16;
};


extern struct encoding * encoding;
extern struct unimap_t unimap[0x80];

void utf8mode (u8 mode);
void init_encoding(void);


#endif /* _ENCODING_H_ */
