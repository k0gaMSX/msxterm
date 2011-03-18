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



static uint8_t lastscan_v;
static uint8_t lastscan_t;
static uint8_t mode;
static uint8_t kbd_matrix[NUMBER_ROW];



void kbd_reset(void)
{
  lastscan_v =  NOKEY;
  mode = MSX_MODE;
  lastscan_v = 0;
  lastscan_t = 0;
  memset(kbd_matrix, 0xff, sizeof(kbd_matrix));
}



int8_t is_kbd(int8_t test)
{
  return 1;
}


void kbd_rstmode(uint8_t mask)
{
  mode &= ~mask;
}



void kbd_setmode(uint8_t mask)
{
  mode &= ~mask;
  mode |= mask;
}



static void kb_ev(uint8_t keycode)
{
  lastscan_v = keycode;
  lastscan_t = 0;

  if (mode & KBD_RAW) {
    put_queue(keycode);
    return;
  }


}


static uint8_t keycode;
static uint8_t * row_pointer;

static void scan_row(uint8_t row)
{
  register uint8_t xor = row ^ *row_pointer;
  register uint8_t mask;

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
  uint8_t i;
  uint8_t row_sel = inp(0xaa) & 0xf0;

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





