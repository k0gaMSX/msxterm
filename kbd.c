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


void kbd_rstmode(u8 mask)
{
  mode &= ~mask;
}



void kbd_setmode(u8 mask)
{
  mode &= ~mask;
  mode |= mask;
}



{


}





static u8 put_queue(u8 scancode)
{

}



static void kb_ev(u8 scancode, u8  up_flag)
{
  lastscan_v = scancode;
  lastscan_t = 0;

  if (mode & KBD_RAW) {
    put_queue(scancode);
    put_queue(!(up_flag != 0));
    return;
  }


}





static void scan_matrix(void)
{
  static u8 i;
  static u8 scancode;
  static u8 * kbnew, * kbold;

  scancode = 0;
  kbnew = (u8 *) NEWKEY, kbold = (u8 *) OLDKEY;

  for (i = 0; i < NUMBER_ROW; ++i) {
    u8 mask = 1;
    u8 xor = *kbold++ ^ *kbnew;

    if (!xor) {
      scancode += 8;
      continue;
    }

    do {
      if (xor & mask)
        kb_ev(scancode, *kbnew & mask);

      ++scancode;
      ++kbnew;
    } while (mask <<= 1);
  }
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





