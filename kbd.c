#include <string.h>
#include <sys.h>
#include <assert.h>

#include "kbd.h"
#include "bell.h"
#include "tty.h"
#include "keymaps.h"

#define NUMBER_ROW        11
#define NOKEY            255
#define RS232_MODE         0
#define MSX_MODE           1
#define KBD_REPEAT_TIME   10


static unsigned char rep;
static unsigned char lastscan_v;
static unsigned char lastscan_t;
static unsigned char mode;
static unsigned char kbd_matrix[NUMBER_ROW];
static unsigned char shift_state;
static unsigned char lock_state;


typedef void k_handler_fn(unsigned char keysym, unsigned char up);
static k_handler_fn k_self, k_meta, k_shift;

static k_handler_fn *k_handlers[16] = {
  k_self, k_self, k_self, k_shift
};




void kbd_reset(void)
{
  rep = 0;
  lastscan_v =  NOKEY;
  shift_state= 0;
  lock_state = 0;
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


static void k_lock(unsigned char value, unsigned char up)
{
  if (!up && !rep)
    lock_state ^= 1<<value;
}


static void k_shift(unsigned char value, unsigned char up)
{
  if (rep)
    return;

  if (up)
    shift_state &= ~(1<<value);
  else
    shift_state |= 1<<value;
}


static void k_self(unsigned char value, unsigned char up)
{
  if (!up) put_queue(value);
}



static void kbd_keycode(unsigned char keycode)
{
  static unsigned short *key_map;
  static unsigned short keysym;
  static unsigned char shift_final, type;

  shift_final = shift_state | (lock_state & ~(1<<KM_SHIFT));
  key_map = key_maps[shift_final];

  assert(key_map);
  keysym = key_map[keycode & 0x7f];

  if ((type = KTYP(keysym)) == KT_LETTER) {       /* only letter symbols */
    type = KT_LATIN;                              /* are affected by capslock*/
    if (lock_state & 1<<KM_SHIFT) {
      key_map = key_maps[shift_final ^ 1<<KM_SHIFT];;
      assert(key_map);
      keysym = key_map[keycode & 0x7f];
    }
  }

 printf("shift = %02x lock = %02x shift_final = %02x\n"
        "keycode = %d keysym = %04x [%d '%c'] "
        "type = %d up = %d\n",
        shift_state, lock_state, shift_final,
        keycode & 0x7f, keysym, KVAL(keysym), KVAL(keysym),
        type, keycode & 0x80);

  assert(k_handlers[type]);
  k_handlers[type](KVAL(keysym), keycode & 0x80);
}






static void kb_ev(unsigned char keycode)
{
  lastscan_v = keycode;
  lastscan_t = 0;

  if (mode & KBD_RAW) {
    put_queue(keycode);
    return;
  } else {
    kbd_keycode(keycode);
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

  rep = 0;
  if (lastscan_t == KBD_REPEAT_TIME) {
    rep = 1;
    lastscan_t = 0;
    put_queue(lastscan_v);
  } else {
    ++lastscan_t;
  }
}





