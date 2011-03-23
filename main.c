#include <stdio.h>
#include <string.h>
#include "vt100.h"
#include "v9990.h"

#define MAXLINE 1024


int main(int argc, char *argv[])
{
  const char str[]="\033cHola \033[33;45mmundo\n";

  init_term();
  vdp_init();

  term_write(str, sizeof(str));

  return 0;
}
