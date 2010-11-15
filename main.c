#include <stdio.h>
#include <string.h>
#include "term.h"

#define MAXLINE 1024


int main(int argc, char *argv[])
{
  char buff[MAXLINE];
  int count;

  puts("\033c");
  init_term();


  while (fgets(buff, MAXLINE, stdin))
    term_write(buff, strlen(buff));

  return 0;
}
