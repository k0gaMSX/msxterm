
#include "tnix/tty.h"
#include "tnix/vt100.h"
#include "tnix/keyboard.h"

static unsigned char ttymode = MSX_TTY;



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
