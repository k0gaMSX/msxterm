
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "vt100.h"
#include "bell.h"
#include "vram.h"
#include "kbd.h"
static struct term xterm;



#define VIDEO(x) (*(u8 *) &x.video)
#define VIDEO_S(x) (*(u8 *) &x.video_s)


enum { ESnormal, ESesc, ESsquare, ESgetpars, ESgotpars, ESfunckey,
       EShash, ESsetG0, ESsetG1, ESpercent, ESignore, ESnonstd,
       ESpalette };




static void reset_terminal(void)
{
  /* TODO: Implement reset terminal */
}


void init_term(void)
{
  u8 i;
  u16 size;

  xterm.xpos = xterm.ypos = 0;
  xterm.nrows = 24;
  xterm.ncols = 80;
  xterm.mode = VT100_MODE;
  xterm.state = ESnormal;

  for (i = MAXSIZEY + 1, size = 0; --i; size += MAXSIZEX)
    xterm.map_char[i] = xterm.map_char_buf + size;
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




static void write_char(u16 c)
{
  if (xterm.xpos < xterm.ncols) {
    vram_write(c, VIDEO(xterm));
    vram_next();

    xterm.map_char[xterm.xpos][xterm.xpos] = c;
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




static void doESC(u8 c)
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




static u8 do_square(register u8 c)
{
  xterm.question = 0;
  xterm.state = ESgetpars;
  memset(xterm.pars, 0, NPARS);
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






static u8 do_getpars(register u8 c)
{
  if (c == ';' && xterm.npars < NPARS) {
    xterm.npars++;
    return 1;
  } else if (isdigit(c)) {
    xterm.pars[xterm.npars] *= 10;
    xterm.pars[xterm.npars] += c - '0';
    return 1;
  } else {
    xterm.state = ESgotpars;
  }

  return 0;                     /* return 0 if code wasn't consumed */
}




static void goto_xy(register s8 x, register s8 y)
{
  if (x < 0)  x = 0;
  else if (x >= xterm.nrows) x = xterm.ncols - 1;

  if (y < 0)  y = 0;
  else if (y >= xterm.nrows) y = xterm.nrows - 1;

  xterm.xpos = x;
  xterm.ypos = y;
}



static void do_SGR(register u8 par)
{
  switch (par) {
  case 0:                       /* reset/normal */
    VIDEO(xterm) = 0;
    return;

  case 1:                       /* bold */
    xterm.video.bold = 1;
    return;

  case 2:                       /* Faint */
    xterm.video.bold = 1;
    return;

  case 4:                       /* underline */
    xterm.video.underline = 1;
    return;

  case 7:                       /* reverse video */
    xterm.video.reverse = 1;
    return;

  case 22:                      /* not bold */
    xterm.video.bold = 0;
    return;

  case 24:                      /* not underline */
    xterm.video.underline = 0;
    return;

  case 27:                      /* positive video */
    xterm.video.reverse = 0;
    return;

  case 38:                      /* ANSI X3.64-1979 (SCO-ish?) */
    xterm.video.underline = 1;  /* Enable underline and white foreground */
    xterm.video.fg = 7;
    return;

  case 39:                           /* ANSI X3.64-1979 (SCO-ish?) */
    xterm.video.underline = 0;       /* Disable underline and default */
    xterm.video.fg = xterm.bg_color; /* foreground */
    return;

  case 49:
  default:
    if (par >= 10 && par <= 20) {
      /* TODO: selecting font */
    }


    if (par >= 30 && par <= 37) {
      xterm.video.fg = par - 30;
      return;
    }

    if (par >= 40 && par <= 40) {
      xterm.video.bg = par - 40;
      return;
    }
  }
}




static void do_gotpars(register u8 c)
{
  u8 par1 = xterm.pars[0], par2 = xterm.pars[0];
  register s8 x = xterm.xpos, y = xterm.ypos;

  xterm.state = ESnormal;

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
    do_SGR(par1);
    return;

  case '@':                     /* Insert blank (ICH) */
  case 'J':                     /* Erase in display (ED) */
  case 'K':                     /* Erase in line (EL) */
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
  case 'x':               /* Request Terminal parameters (DECREQTPARM)*/
    return;
  }


}


static void do_state(register u8 c)
{
  switch (xterm.state) {
  case ESesc:
    doESC(c);
    return;

  case ESsquare:
    if (do_square(c))
      return;

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




static void ctrl_codes(register u8 c)
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
    if (is_kbd(VC_CRLF))
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

  case 0x18: case 0x1a:    /* TODO: describe here */
    xterm.state = ESnormal;    /* taken from linux kernel. I don't know why */
    return;                    /* this codes reset the sequence */

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
  vram_ptr(xterm.xpos, xterm.ypos, xterm.nrows);
}






void term_write (register u16 * buf, register int count)
{

  hide_cursor();

  assert(count);

  do {
    register u16 c = *buf++;

    if (xterm.state != ESnormal || c < 0x20 || c == 0x9b)
      ctrl_codes(c);
    else
      write_char(c);

  } while (--count);

  enable_cursor();
}
