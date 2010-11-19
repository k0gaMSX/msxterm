#include <memory.h>
#include "encoding.h"

struct encoding * encoding;      /* struct with pointer to code/decode
                                   methods in actual encoding */

struct unimap_t unimap[0x80];   /* Array for translation between 8 bits
                                          encodings and unicode */



enum {
  UTF8_1BYTE, UTF8_2BYTE, UTF8_3BYTE_1, UTF8_3BYTE_2};



static u8 status = UTF8_1BYTE;
static u16 val = 0;

static void utf8reset(void);
static u8 utf8decode(u8 in, u16 * out);
static u8 utf8encode(u16 in, u8 * out);
static void reset(void);
static u8 decode(u8 in, u16 * out);
static u8 encode(u16 in, u8 * out);

static struct encoding utf8 = {utf8reset, utf8decode, utf8encode};
static struct encoding non_utf8 = {reset, decode, encode};




void init_encoding(void)
{
  memset(unimap, sizeof(unimap), 0);

  ut8reset();
}


void utf8mode (u8 mode)
{
  if (!mode) {
    encoding = &utf8;
    utf8reset();
  } else {
    encoding = &non_utf8;
  }
}




static void utf8reset(void)
{
  status = UTF8_1BYTE;
  val = 0;
}



static u8 utf8decode(u8 in, u16 * out)
{
  switch (status) {
  case UTF8_1BYTE:
    if (!(in & 0x80)) {
      *out = in;
      return 1;
    }

    if ((in & 0xe0) == 0xc0) {
      val = (u16) (in & 0x1f) << 6;
      status = UTF8_2BYTE;
      return 0;
    }

    if ((in & 0xf0) == 0xe0) {
      val = (u16) (in & 0x0f) << 12;
      status = UTF8_3BYTE_1;
      return 0;
    }
    break;

  case UTF8_2BYTE:
    if ((in & 0xc0) == 0x80) {
      *out = val | (u16) (in & 0x3f);
      status = UTF8_1BYTE;
      val = 0;
      return 1;
    }
    break;

  case UTF8_3BYTE_1:
    if ((in & 0xc0) == 0x80) {
      val |= (u16) (in & 0x3f) << 6;
      status = UTF8_3BYTE_2;
      return 0;
    }
    break;

  case UTF8_3BYTE_2:
    if ((in & 0xc0) == 0x80) {
      *out  = val | (in & 0x3f);
      status = UTF8_1BYTE;
      val = 0;
      return 1;
    }
    break;
  }


  val = 0;
  status = UTF8_1BYTE;
  *out = UCS_INVALID;
  return 1;
}



static u8 utf8encode(u16 in, u8 * out)
{

  if ( (u8) (in >> 8) & 0xf8) {
    out[2] = 0x80 | (u8) in & 0x3f;
    in >>= 6;
    out[1] = 0x80 | (u8) in & 0x3f;
    in >>= 6;
    out[0] = 0xe0 | (u8) in & 0x3f;

    return 3;
  }

  if (in & 0x0780) {
    out[1] = 0x80 | (u8) in &0x3f;
    in >>= 6;
    out[0] = 0xc0 | (u8) in & 0x3f;
    return 2;
  }

  *out = (u8) in;
  return 1;
}






static void reset(void)
{
}


static u8 decode(u8 in, u16 * out)
{
  if (in & 0x80)
    *out = unimap[in & 0x7f].bit16;
  else
    *out = in;

  return 1;
}


static u8 encode(u16 in, u8 * out)
{
  if (in < 0x80) {
    *out = in;
  } else {
    u8 i;
    struct unimap_t * ptr;

    for (i = 0x80, ptr = unimap; --i; ++ptr) {
      u8 bit8;

      if (!(bit8 = ptr->bit8))
        break;

      if (ptr->bit16 == in) {
        *out = bit8;
        return 1;
      }
    }

    *out = ASCII_INVALID;
  }

  return 1;
}
