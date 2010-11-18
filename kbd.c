#include <string.h>
#include <msx.h>
#include "kbd.h"
#include "bell.h"

#define NUMBER_ROW        11
#define NOKEY             -1
#define RS232_MODE         0
#define MSX_MODE           1
#define KBD_REPEAT_TIME   10




static u8 lastscan_v;
static u8 lastscan_t;
static u8 mode;




void kbd_reset(void)
{
  lastscan_v =  NOKEY;
  mode = MSX_MODE;
}



u8 is_kbd(u8 test)
{
  return 1;
}


void kbd_rs232(u8 flag)
{
  mode = (flag) ? RS232_MODE : MSX_MODE;
}




static u8 put_queue(u8 scancode)
{
}



static void kb_ev(u8 scancode)
{
  if (lastscan_v == scancode) {  /* it is a release */
    lastscan_v = NOKEY;
    return;
  }

  if (!put_queue(lastscan_v = scancode))  /* buffer overflow!!! */
    bell();

  lastscan_t = 0;
}





static u8 scan_row(register u8 xor, u8 scancode)
{
  u8 j;

  if (!xor)
    return scancode + 8;        /* there isn't a new key */

  for (j = 9; --j; ++scancode, xor >>= 1) {
    if (xor & 1)         /* event key */
      kb_ev(scancode);
  }

  return scancode;
}



static void scan_matrix(void)
{
  register u8 i;
  static u8 scancode;
  static u8 * kbnew, * kbold;

  scancode = 0;
  kbnew = (u8 *) NEWKEY, * kbold = (u8 *) OLDKEY;

  for (i = 0; i < NUMBER_ROW; ++i)
    scancode = scan_row(*kbnew++ ^ *kbold++, scancode);
}





static void kbd_hook(void)
{
  scan_matrix();

  if (lastscan_t == KBD_REPEAT_TIME) {
    put_queue(lastscan_v);
    lastscan_t = 0;
  } else {
    ++lastscan_t;
  }
}





