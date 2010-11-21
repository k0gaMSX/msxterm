#include <stdlib.h>
#include <stdio.h>
#include <intrpt.h>
#include "kbd.h"
#include "tty.h"


static int count;

void put_queue(u8 val)
{
  count = 0;
  printf ("%d - %02x\n", val, val);
}



int main(int argc, char *argv[])
{
  kbd_reset();
  kbd_setmode(KBD_RAW);

  for (;;) {
    di();
    scan_matrix();

    if (++count == 10*50) {
      break;
    }

    ei();
    halt();
  }

  return EXIT_SUCCESS;
}
