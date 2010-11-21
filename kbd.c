#include <string.h>
#include <sys.h>

#include "kbd.h"
#include "bell.h"
#include "tty.h"


#define NUMBER_ROW        11
#define NOKEY            255
#define RS232_MODE         0
#define MSX_MODE           1
#define KBD_REPEAT_TIME   10



static u8 lastscan_v;
static u8 lastscan_t;
static u8 mode;
static u8 kbd_matrix[NUMBER_ROW];



void kbd_reset(void)
{
  lastscan_v =  NOKEY;
  mode = MSX_MODE;
  lastscan_v = 0;
  lastscan_t = 0;
  memset(kbd_matrix, 0xff, sizeof(kbd_matrix));
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



static void kb_ev(u8 keycode)
{
  lastscan_v = keycode;
  lastscan_t = 0;

  if (mode & KBD_RAW) {
    put_queue(keycode);
    return;
  }


}


static u8 keycode;
static u8 * row_pointer;

static void scan_row(u8 row)
{
  register u8 xor = row ^ *row_pointer;
  register u8 mask;

  *row_pointer++ = row;
  if (!xor) {
    keycode += 8;
    return;
  }

  for (mask= 1; mask; mask <<= 1) {
    if (xor & mask)
      kb_ev(keycode++ | ((row & mask) ? 0x80 : 0));

    ++keycode;
  }
}


void scan_matrix(void)
{
  u8 i;
  u8 row_sel = inp(0xaa) & 0xf0;

  row_pointer = kbd_matrix;
  keycode = 0;
  for (i = 0; i < NUMBER_ROW; ++i) {
    outp(0xaa, row_sel | i);
    scan_row(inp(0xa9));
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





