#include <string.h>
#include <sys.h>
#include <assert.h>

#include "keyboard.h"
#include "bell.h"
#include "tty.h"
#include "keymaps.h"



static unsigned char kbd_mode;
static unsigned char shift_state;
static unsigned char lock_state;


typedef void k_handler_fn(unsigned char keysym, unsigned char up);
static k_handler_fn k_self, k_meta, k_shift;

static k_handler_fn *k_handlers[16] = {
  k_self, k_self, k_self, k_shift
};




unsigned char is_kbd(unsigned char test)
{
  return 1;
}


void kbd_rstmode(unsigned char mask)
{
  kbd_mode &= ~mask;
}



void kbd_setmode(unsigned char mask)
{
  kbd_mode &= ~mask;
  kbd_mode |= mask;
}


static void k_lock(unsigned char value, unsigned char up)
{
     if (!up)
    lock_state ^= 1<<value;
}


static void k_shift(unsigned char value, unsigned char up)
{
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



void kbd_event(unsigned char keycode)
{
  if (kbd_mode & KBD_RAW)
    put_queue(keycode);
  else
    kbd_keycode(keycode);
}
















