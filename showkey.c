#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <intrpt.h>

#include "kbd.h"
#include "tty.h"
#include "version.h"

#define TO_SEC(x) (50*x)

enum {KEYCODES, KEYMAP, UNICODE};
static int count;
static uint8_t command = KEYCODES;
static int delay = TO_SEC(10);        /* Default timeout */




void put_queue(uint8_t val)
{
  count = 0;
  if (command == KEYCODES)
    printf ("%d\n", val);
  else if (command == KEYMAP)
    ;                                /* TODO: Implement this */
  else
    printf("%c - %d\n", val, val);   /* TODO: Implement unicode mode */
}


void usage(void)
{
  puts("usage: showkey [options...] <command>");
  puts("valid commands are:");
  puts("\t -k display only the interpreted keycodes (default).");
  puts("\t -m display only keymap-translated chars.");
  puts("\t -u display unicode-translated chars.");
  puts("\t -h display this help text and exit");
  puts("\t -v display version information and exit");
  puts("options are:");
  puts("\t -t=N  set the timeout to N seconds.");
}


void options(int argc, char * argv[])
{
 switch (argc) {
  case 3:
    ++argv;
    if (strcmp(*argv, "-t="))
      break;
    else
      delay = TO_SEC(atoi(*argv + 3));

  case 2:
    ++argv;
    if (**argv != '-' || strlen(*argv) != 2)
      break;

    switch ((*argv)[1]) {
    case 'k':
      command = KEYCODES;
      kbd_setmode(KBD_RAW);
      return;

    case 'm':
      command = KEYMAP;
      return;

    case 'u':
      command = UNICODE;
      return;

    case 'h':
      usage();
      exit(EXIT_SUCCESS);
    case 'V':
      printf("showkey: %s\n", VERSION);
      exit(EXIT_SUCCESS);
    }
    break;

  case 1:
    kbd_setmode(KBD_RAW);
    return;

  default:
    break;
  }

 usage();
 exit(EXIT_FAILURE);
}



int main(int argc, char *argv[])
{
  kbd_reset();
  options(argc, argv);

  for (;;) {
    di();
    scan_matrix();

    if (++count == delay) {
      break;
    }

    ei();
    halt();
  }

  return EXIT_SUCCESS;

}
