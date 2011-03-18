#ifndef _ENCODING_H_
#define _ENCODING_H_
#include "types.h"

#define UCS_INVALID 0xfffd
#define ASCII_INVALID 0x1a

struct encoding {
  void (* reset)(void);
  uint8_t (* decode)(uint8_t in, uint16_t * out);
  uint8_t (* encode)(uint16_t in, uint8_t * out);
};




struct unimap_t {
  uint8_t bit8;
  uint16_t bit16;
};


extern struct encoding * encoding;
extern struct unimap_t unimap[0x80];

void utf8mode (uint8_t mode);
void init_encoding(void);


#endif /* _ENCODING_H_ */
