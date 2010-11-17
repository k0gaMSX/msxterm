#ifndef _ENCODING_H_
#define _ENCODING_H_
#include "types.h"

#define UCS_INVALID 0xfffd


struct encoding {
  void (* reset)(void);
  u8 (* decode)(u8 in, u16 * out);
  u8 (* encode)(u16 in, u8 * out);
};


extern struct encoding encoding;


#endif /* _ENCODING_H_ */
