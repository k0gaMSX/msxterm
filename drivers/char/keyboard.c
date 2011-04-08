
#include "arch/string.h"

#include "tnix/assert.h"
#include "tnix/keyboard.h"
#include "tnix/bell.h"
#include "tnix/tty.h"
#include "tnix/keymaps.h"



static unsigned char kbd_mode;
static unsigned char shift_state;
static unsigned char lock_state;


typedef void k_handler_fn(unsigned char keysym, unsigned char up);
static k_handler_fn k_self, k_meta, k_shift, k_cur;

static k_handler_fn *k_handlers[16] = {
     k_self, k_self, k_self, k_shift, k_self, k_cur
};




unsigned char is_kbd(unsigned char test)
{
     return test & kbd_mode;
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


static void applkey(unsigned char key, char mode)
{
     static char buff[] = {0x1b, 0, 0, 0};

     buff[1] = (mode ? '0' : '[');
     buff[2] = key;
     puts_queue(buff, 3);
}



static void k_cur(unsigned char value, unsigned char up)
{
     if (!up)
          applkey(value, 0);
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
          type = KT_LATIN;                      /* are affected by capslock*/
          if (lock_state & 1<<KM_SHIFT) {
               key_map = key_maps[shift_final ^ 1<<KM_SHIFT];;
               assert(key_map);
               keysym = key_map[keycode & 0x7f];
          }
     }


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
















