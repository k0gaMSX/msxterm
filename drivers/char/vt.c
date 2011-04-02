
#include "arch/string.h"
#include "arch/vram.h"

#include "tnix/ctype.h"
#include "tnix/assert.h"
#include "tnix/vt.h"
#include "tnix/bell.h"
#include "tnix/keyboard.h"

static struct term xterm;

#define ERASE_CHAR '\0'

enum { ESnormal, ESesc, ESsquare, ESgetpars, ESgotpars, ESinpars,
       ESfunckey, EShash, ESsetG0, ESsetG1, ESpercent, ESignore,
       ESnonstd, ESpalette };


#define DEFAULT_BG_COLOR 0
#define DEFAULT_FG_COLOR 7

static void reset_terminal(void)
{
  clean_vram();
  xterm.xpos = xterm.ypos = 0;
  CLEAR_VIDEO(xterm.video);
  xterm.video.fg = xterm.fg_color;
  xterm.video.bg = xterm.bg_color;
  xterm.video_s = xterm.video;
  ptr_vram(0, 0);
  {unsigned short **bp;
   for (bp  = xterm.map_char; bp < xterm.map_char + MAXSIZEY; ++bp)
     memset(*bp, MAXSIZEX * sizeof(*bp), ERASE_CHAR);
  }
  {struct video_att **bp;
   for (bp = xterm.map_video; bp < xterm.map_video + MAXSIZEY; ++bp)
     memset(*bp, MAXSIZEX * sizeof(*bp), 0);
  }

  xterm.state = ESnormal;
  enable_cursor();
}


void con_init(void)
{
  unsigned char i;
  register size_t  size;

  xterm.xpos = xterm.ypos = 0;
  xterm.nrows = 24;
  xterm.ncols = 80;
  xterm.mode = VT100_MODE;
  xterm.state = ESnormal;
  xterm.bg_color = DEFAULT_BG_COLOR;
  xterm.fg_color = DEFAULT_FG_COLOR;

  for (i = size = 0; i < MAXSIZEY; ++i, size += MAXSIZEX) {
    xterm.map_char[i] = xterm.map_char_buf + size;
    xterm.map_video[i] = xterm.map_video_buf + size;
  }

  ptr_vram(0, 0);
}


/* TODO: deal with wrap seetings */



static void bs(void)
{
  if (xterm.xpos) {
    --xterm.xpos;
/*    xterm.att.vc_need_wrap = 0;                                         */
  }
}



static void tab(void)
{
  /* TODO: Implement TABs */
/*    xterm.att.vc_need_wrap = 0;                                         */

}




static void lf(void)
{
  if (xterm.ypos == xterm.nrows) {
    ;/* do scrolling ... */
  } else {
    ++xterm.ypos;
  }

/*  xterm.att.vc_need_wrap = 0;                                           */
}



static void cr(void)
{
  xterm.xpos = 0;
/*  xterm.att.vc_need_wrap = 0;                                           */
}


static void del(void)
{
  /* Ignored */
}



static void ri()
{
  /* TODO: implement RI (reverse index) */
}




static void write_char(unsigned char c)
{
  if (xterm.xpos < xterm.ncols) {
    write_vram(c, xterm.video);
    next_vram();

    xterm.map_char[xterm.xpos][xterm.xpos] = c;
    xterm.map_video[xterm.ypos][xterm.xpos] = xterm.video;
    ++xterm.xpos;
  }
}



static void put_ID(void)
{
  /* TODO: Implement DECID*/
}


static void save_cur(void)
{
  xterm.xpos_s = xterm.xpos;
  xterm.ypos_s = xterm.ypos;
  xterm.video_s = xterm.video;
}



static void restore_cur(void)
{
  xterm.xpos = xterm.xpos_s;
  xterm.ypos = xterm.ypos_s;
  xterm.video = xterm.video_s;
}




static void doESC(unsigned char c)
{
  xterm.state = ESnormal;
  switch(c) {
  case '[':
    xterm.state = ESsquare;
    return;

  case ']':
    xterm.state = ESnonstd;
    return;

  case '%':
    xterm.state = ESpercent;
    return;

  case 'E':                     /* Next line (NEL) */
    cr();
    lf();
    return;

  case 'M':                     /* Reverse Index (RI) */
    ri();
    return;

  case 'D':                     /* Index (IND) */
    lf();
    return;

  case 'H':                     /* Tab set (HTS) */
    return;                     /* TODO: implement TABS */

  case 'Z':                     /* Return terminal ID (DECID) */
    put_ID();
    return;

  case '7':                     /* Save cursor (DECSC) */
    save_cur();
    return;

  case '8':                     /* Restore cursor (DECRC) */
    restore_cur();
    return;

  case 'c':                     /* Full reset (RIS) */
    reset_terminal();
    return;

  case '(':                     /* Designate G0 character set (ISO 2022) */
    /* TODO: G0 */
  case ')':                     /* Designate G1 character set (ISO 2022) */
    /* TODO: G1 */
  case '*':                     /* Designate G2 character set (ISO 2022) */
    /* TODO: G2 */
  case '+':                     /* Designate G3 character set (ISO 2022) */
    /* TODO: G3 */


  case '#':                     /* DEC screen alignement test (DECALN) */
    /* TODO: DEC test */
  case '>':                     /* Normal keypad (DECPNM) */
    /* TODO: Normal Keypad */
  case '=':                     /* Application keypad (DECPAM) */
    /* TODO: Application keypad */
    return;
  }
}




static unsigned char do_square(register unsigned char c)
{
  xterm.question = 0;
  xterm.state = ESgetpars;
  memset(xterm.pars, 0, sizeof(xterm.pars));
  xterm.npars = 0;

  if (c == '[') {
    xterm.state = ESfunckey;
    return 1;
  } else if (c == 'N') {
    return 1;                   /* TODO: SS2 */
  } else if (c == 'O') {
    return 1;                   /* TODO: SS3 */
  } else if (c == '?') {
    xterm.question = 1;
    return 1;
  }

  return 0;                       /* return 0 if code wasn't consumed */
}






static unsigned char do_getpars(register unsigned char c)
{
  if (c == ';' && xterm.npars < NPARS) {
    xterm.npars++;
    xterm.state = ESgetpars;
    return 1;
  } else if (isdigit(c)) {
    xterm.pars[xterm.npars] *= 10;
    xterm.pars[xterm.npars] += c - '0';
    xterm.state = ESinpars;
    return 1;
  } else {
    if (xterm.state == ESinpars)
      ++xterm.npars;
    xterm.state = ESgotpars;
  }

  return 0;                     /* return 0 if code wasn't consumed */
}



static void goto_xy(register signed char x, register signed char y)
{
  if (x < 0)  x = 0;
  else if (x >= xterm.nrows) x = xterm.ncols - 1;

  if (y < 0)  y = 0;
  else if (y >= xterm.nrows) y = xterm.nrows - 1;

  xterm.xpos = x;
  xterm.ypos = y;
}



static void do_SGR(void)
{
  unsigned char *ptr = xterm.pars;
  while (xterm.npars--) {
    register unsigned char par = *ptr++;
      switch (par) {
      case 0:                       /* reset/normal */
        CLEAR_VIDEO(xterm.video);
        break;

      case 1:                       /* bold */
        xterm.video.bold = 1;
        break;

      case 2:                       /* Faint */
        xterm.video.bold = 1;
        break;

      case 4:                       /* underline */
        xterm.video.underline = 1;
        break;

      case 7:                       /* reverse video */
        xterm.video.reverse = 1;
        break;

      case 22:                      /* not bold */
        xterm.video.bold = 0;
        break;

      case 24:                      /* not underline */
        xterm.video.underline = 0;
        break;;

      case 27:                      /* positive video */
        xterm.video.reverse = 0;
        break;


      case 38:                      /* ANSI X3.64-1979 (SCO-ish?) */
        xterm.video.underline = 1;  /* Enable underline and white foreground */
        xterm.video.fg = 7;
        break;

      case 39:                           /* ANSI X3.64-1979 (SCO-ish?) */
        xterm.video.underline = 0;       /* Disable underline and default */
        xterm.video.fg = xterm.bg_color; /* foreground */
        break;;

      case 49:
      default:
        if (par >= 10 && par <= 20) {
          /* TODO: selecting font */
        }

        if (par >= 30 && par <= 37) {
          xterm.video.fg = par - 30;
          break;
        }

        if (par >= 40 && par <= 47) {
          xterm.video.bg = par - 40;
          break;
        }
      }
  }
}




static void scroll_region(unsigned char par1, unsigned char par2)
{
  if (!par1)  ++par1;
  if (!par2)  par2 = xterm.nrows;

  if (par1 < par2 && par2 <= xterm.nrows) { /* Minimum allowed region */
    xterm.top = par1;                       /*is 2 lines */
    xterm.botton = par2;
    /* TODO: Cursor must go to home position */
  }
  return;
}



static insert_char(unsigned char val, unsigned char count)
{
  unsigned char offset = xterm.ncols - count, x = xterm.xpos + count;
  unsigned short *src = xterm.map_char[xterm.ypos] + offset - 1;
  struct video_att * video = xterm.map_video[xterm.ypos] + offset - 1;


  assert(count > 0 && offset > 0);
  ptr_vram(xterm.ncols - 1, xterm.ypos);

  do {
    write_vram(*src, *video);
    prev_vram();

    xterm.map_char[xterm.xpos][x] = *src--;
    xterm.map_video[xterm.ypos][x--] = *video--;
  } while (--offset);

  ptr_vram(xterm.xpos, xterm.ypos);
  do
    write_char(val);
  while (--count);
}




static void erase_in_line(unsigned char par1)
{
  unsigned short *start;
  size_t  count;

  switch (par1){
  case 0:
    start = &xterm.map_char[xterm.ypos][xterm.xpos];
    count = MAXSIZEX - xterm.ypos;
    break;
  case 1:
    start = &xterm.map_char[xterm.ypos][0];
    count = xterm.xpos;
    break;
  case 2:
    start = &xterm.map_char[xterm.ypos][0];
    count = MAXSIZEX;
  default:
    return;
  }

  memset(start, ERASE_CHAR, count * sizeof(*start));
}





static void erase_in_display(unsigned char par1)
{
  unsigned short *start;
  unsigned count;


  switch (par1){
  case 0:
    start = &xterm.map_char[xterm.ypos][xterm.xpos];
    count = xterm.map_char_buf - start - 1;
    break;
  case 1:
    start = xterm.map_char_buf;
    count = &xterm.map_char[xterm.ypos][xterm.xpos] - start;
    break;
  case 2:
    start = xterm.map_char_buf;
    count = sizeof(xterm.map_char_buf) / sizeof(*xterm.map_char_buf);
  default:
    return;
  }

  memset(start, ERASE_CHAR, count * sizeof(*start));
}



static void do_gotpars(register unsigned char  c)
{
  unsigned char par1 = xterm.pars[0], par2 = xterm.pars[0];
  register signed char x = xterm.xpos, y = xterm.ypos;

  xterm.state = ESnormal;
  if (!xterm.question) {
    /* TODO: Implement operations */
    switch (c) {
    case 'A':                     /* Cursor up (CUU) */
      if (!par1)  ++par1;
      y -= par1;
      goto_xy(x,y);
      return;

    case 'B':                     /* Cursor down (CUD) */
      if (!par1) ++par1;
      y += par1;
      goto_xy(x,y);
      return;

    case 'C':                     /* Cursor forward (CUF) */
      if (!par1) ++par1;
      y += par1;
      goto_xy(x,y);
      return;

    case 'D':                     /* Cursor backward (CUB) */
      if (!par1) ++par1;
      y += par1;
      goto_xy(x,y);
      return;

    case 'H':                     /* Cursor position (CUP) */
      if (!par1) ++par1;
      if (!par2) ++par2;
      x = par1;
      y = par2;
      goto_xy(x,y);
      return;

    case 'm':                     /* Character attributes (SGR) */
      do_SGR();
      return;

    case '@':                     /* Insert blank (ICH) */
    case 'J':                     /* Erase in display (ED) */
      erase_in_display(par1);
      return;

    case 'K':                     /* Erase in line (EL) */
      erase_in_line(par1);
      return;

    case 'L':                     /* Insert lines (IL) */
    case 'M':                     /* Delete lines (DL) */
    case 'P':                     /* Delete characters (DCH) */
    case 'T':                     /* Initiate hilite mouse tracking */
    case 'c':                     /* Send device attributes (DA) */
    case 'f':                     /* Horizontal and vertical pos (HVP) */
    case 'g':                     /* Tab clear (TBC) */
    case 'h':                     /* Set mode (SM) */
    case 'l':                     /* Reset mode (RM) */
    case 'n':                     /* Device status report (DSR) */

    case 'r':                     /* Set scrolling Region (DECSTBM) */
      scroll_region(par1, par2);
      return;

    case 'x':               /* Request Terminal parameters (DECREQTPARM)*/
      return;
    }
  } else {
    switch (c) {
    case 'h':                   /* DEC Private mode set (DECSET) */
    case 'l':                   /* DEC Private mode reset (DECRST) */
    case 'r':                   /* Restore DEC Private mode values */
    case 's':                   /* Save DEC Private mode values */
      return;
    }
  }
}


static void do_state(register unsigned char c)
{
  switch (xterm.state) {
  case ESesc:
    doESC(c);
    return;

  case ESsquare:
    if (do_square(c))
      return;

  case ESinpars:
  case ESgetpars:
    if (do_getpars(c))
      return;

  case ESgotpars:
    do_gotpars(c);
    return;

    /* TODO: Implement this states */
  case ESfunckey:
  case EShash:
  case ESsetG1:
  case ESsetG0:
  case ESpercent:
  case ESignore:
  case ESnonstd:
  case ESpalette:
    break;
  }
}




static void ctrl_codes(register unsigned char c)
{
  switch (c) {
  case 0x00:
    return;
  case 0x07:              /* BEL */
    bell();
    return;

  case 0x08:              /* BS */
    bs();
    goto act_ptr;

  case 0x09:              /* TAB */
    tab();
    goto act_ptr;

  case 0x0a:              /* LF */
    lf();
    if (is_kbd(KBD_CRLF))
      goto act_ptr;

  case 0x0b:              /* CR */
    cr();
    goto act_ptr;

  case 0x0e:              /* SO, set G1 charset */
  case 0x0f:              /* SI, set G0 charset */
    /* TODO: Implement SI and SO */
    break;

  case 0x7f:              /* DEL */
    del();
    goto act_ptr;

  case 0x18:                 /* CAN character */
  case 0x1a:                 /* SUB character */
    xterm.state = ESnormal;  /* they must finish actual sequence */
    return;

  case 0x1b:                  /* ESC */
    xterm.state = ESesc;
    return;

  case 0x9b:                  /* meta ESC  */
    xterm.state = ESsquare;
    return;
  }

  do_state(c);

  if (xterm.state != ESnormal)  /* we are inside a sequence yet */
    return;

 act_ptr:
  ptr_vram(xterm.xpos, xterm.ypos);
}






void con_write (const void *buf, unsigned count)
{
  const unsigned char *bp = (unsigned char *) buf;
  assert(count && buf);
  hide_cursor();

  do {
    unsigned char c = *bp++;

    if (xterm.state != ESnormal || c < 0x20 || c == 0x9b)
      ctrl_codes(c);
    else
      write_char(c);

  } while (--count);

  enable_cursor();
}
