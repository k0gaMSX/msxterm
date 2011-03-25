#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "config.h"
#include "vram.h"



#ifdef HAVE_SYS_STAT_H
   #include <sys/stat.h>
#endif

#ifdef HAVE_SYS_TYPES_H
   #include <sys/types.h>
#endif

#ifdef HAVE_UNISTD_H
   #include <unistd.h>
#endif

#ifdef HAVE_INTTYPES_H
  #include <inttypes.h>
#else
  #include "types.h"
#endif

static const char version_string[] = CONSOLECHARS_VERSION;
static const char help_string[] =
  "conchar  [-V|--version]  [-h|--help]\n"
  "          [-H|--char-height=N]  [-F|--old-font-raw=font.orig]\n"
  "          [-f|--font=font.new]\n"
  "Test con 'conchar --help' for more information\n";

static const char more_help_string[] =
  "-V, --version         Print conchar version\n"
  "-h                    Print a brief command description\n"
  "--help                Print this text\n"
  "-H, --char-height=N   Usually height font is calculated from file size\n"
  "                      but this option lets you indicate other value. N\n"
  "                      should be a value between 8 and 16\n"
  "-f, --font=file       Name of the file from is read the fonts (Mandatory)\n"
  "-F, --old-font-raw=file   Name of the file where old font is saved\n";


static const char *new_font;
static const char *old_font;
static unsigned char char_height;
static unsigned char buf_font[16 * 256];





static void show_version(void)
{
  puts(version_string);
  exit(EXIT_SUCCESS);
}



static die(char * fmt, ...)
{
  va_list va;

  assert(fmt);
  va_start(va, fmt);
  fputs("conchar: ", stderr);
  vfprintf(stderr, fmt, va);
  exit(EXIT_FAILURE);
}


static void usage(void)
{
  puts(version_string);
  puts(help_string);
  exit(EXIT_SUCCESS);
}


static void long_usage(void)
{
  puts(version_string);
  puts(help_string);
  puts(more_help_string);
  exit(EXIT_SUCCESS);
}



static void parse_args(char *argv[])
{
  static char *option;
  static char *char_height_string;

  assert(argv);
  while (option = *++argv) {
    char need_arg = 0, invalid_option = 1;
    char *arg = NULL;

    if (*option++ == '-') {
      if (*option == '-') {             /* -- long args */
        invalid_option = 0;
        ++option;
        /* show_version and long_usage doesn't return */
        if (!strcmp(option, "version"))           show_version();
        else if (!strcmp(option, "help"))         long_usage();

        need_arg = 1;
        if (arg = strchr(option, '='))
          *arg++ = '\0';

        if (!strcmp(option, "char-height"))       char_height_string = arg;
        else if (!strcmp(option, "font"))         new_font = arg;
        else if (!strcmp(option, "old-font-raw")) old_font = arg;
        else                                      invalid_option = 1;

      } else if (option[1] == '\0') {             /* - short args */
        char short_option = option[0];

        invalid_option = 0;
        /* show_version and usage doesn't return */
        if (short_option == 'V')                  show_version();
        else if (short_option == 'h')             usage();

        need_arg = 1;
        if (argv[1][0] != '-')  /* next argv value is an argument? */
          arg = *++argv;

        switch (*option) {
        case 'H':
          char_height_string = arg;
          break;
        case 'f':
          new_font = arg;
          break;
        case 'F':
          old_font = arg;
          break;
        default:
          invalid_option = 1;
          break;
        }
      }
    }

    if (invalid_option)
      die("Incorrect option: %s\n", option);

    if (need_arg && !arg)
      die("%s option needs a parameter\n", option);
  }

  if (new_font == NULL)
    die("-f, --new-font option is mandatory\n");

  if (char_height_string) {
    char_height = atoi(char_height_string);
    if (char_height != 8  && char_height != 16)
      die("-H, --char-height needs a value between 8 and 16\n");
    }
}

/**
 * calculate the height of the glyphs in base of the file
 **/

static unsigned char get_height(const char * fname, FILE * fp)
{
  size_t fsize = 0;
  unsigned char height;

#ifdef HAVE_STAT
  struct stat buf;

  assert(fname);
  if (!stat(fname, &buf))
    fsize = buf.st_size;
#else
  assert(fp);
  if (!fseek(fp, 0, SEEK_END)) {
    fsize = ftell(fp);
    rewind(fp);
  }
#endif

  if (!fsize)                   /* there was a problem */
    die("I can't get size of %s file: %s\n", new_font, strerror(errno));

  height = fsize/256;
  if (height < 8  || height > 16)
       die("Incorrect font file size\n");

  return height;
}



static void save_fonts(const char * fname)
{
  FILE *fp;
  unsigned char height;

  assert(fname);
  fp = fopen(fname, "w");

  if (!fp)
    die("Error opening output font file: %s\n", strerror(errno));

  height = read_glyphs(buf_font);

  if (fwrite(buf_font, height, 256, fp) != 256 || fclose(fp))
    die("Error writing output file: %s\n", strerror(errno));
}




static void load_fonts(const char * fname, unsigned char height)
{
  FILE *fp;

  assert(new_font);
  fp = fopen(new_font, "r");

  if (!fp)
    die("Error opening input font file: %s\n", strerror(errno));

  if (!height)
    height = get_height(fname, fp);

  if (fread(buf_font, height, 256, fp) != 256)
    die("Error reading input font file\n");

  write_glyphs(buf_font, height);
  fclose(fp);
}



int main (int argc, char *argv[])
{
  parse_args(argv);

  if (old_font)
    save_fonts(old_font);

  load_fonts(new_font, char_height);

  return EXIT_SUCCESS;
}
