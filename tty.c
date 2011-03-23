
#include "tty.h"
#include "vt100.h"
#include "keyboard.h"

static unsigned char ttymode;



void tty(void)
{
  extern void init_term(void), vdp_init(void);

  ttymode = MSX_TTY;
  init_term();
  vdp_init();
}



void tty_mode(unsigned char mode)
{
  ttymode |= mode;
}


void put_queue(unsigned char c)
{
  if (ttymode == MSX_TTY)
    term_write(&c, 1);

}




void puts_queue(unsigned char *str, unsigned count)
{
  if (ttymode == MSX_TTY)
    term_write(str, count);

}
