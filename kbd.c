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



static unsigned char lastscan_v;
static unsigned char lastscan_t;
static unsigned char mode;
static unsigned char kbd_matrix[NUMBER_ROW];



void kbd_reset(void)
{
  lastscan_v =  NOKEY;
  mode = MSX_MODE;
  lastscan_v = 0;
  lastscan_t = 0;
  memset(kbd_matrix, 0xff, sizeof(kbd_matrix));
}



unsigned char is_kbd(unsigned char test)
{
  return 1;
}


void kbd_rstmode(unsigned char mask)
{
  mode &= ~mask;
}



void kbd_setmode(unsigned char mask)
{
  mode &= ~mask;
  mode |= mask;
}



static void kb_ev(unsigned char keycode)
{
  lastscan_v = keycode;
  lastscan_t = 0;

  if (mode & KBD_RAW) {
    put_queue(keycode);
    return;
  }


}


static unsigned char keycode;
static unsigned char *row_pointer;


static void scan_row(unsigned char row)
{
  register unsigned char mask, xor = row ^ *row_pointer;

  *row_pointer++ = row;
  if (!xor) {
    keycode += 8;
    return;
  }

  for (mask= 1; mask; mask <<= 1) {
    if (xor & mask)
      kb_ev(keycode | ((row & mask) ? 0x80 : 0));

    ++keycode;
  }
}


void scan_matrix(void)
{
  register unsigned char i;
  static unsigned char row_sel;

  row_pointer = kbd_matrix;
  row_sel = inp(0xaa) & 0xf0;
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





