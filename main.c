#include <stdio.h>
#include <string.h>
#include "vt100.h"

#define MAXLINE 1024


int main(int argc, char *argv[])
{
  unsigned short buff[] = {'h', 'o'};

  init_term();
  term_write(buff, sizeof(buff));

  return 0;
}
