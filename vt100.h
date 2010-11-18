
#ifndef _VT100_H_
#define _VT100_H_



#include "types.h"




#if 0
  unsigned vc_charset        : 1;  /* Character set G0 / G1 */
  unsigned vc_s_charset      : 1;  /* Saved character set */
  unsigned vc_disp_ctrl      : 1;  /* Display chars < 32? */
  unsigned vc_toggle_meta    : 1;  /* Toggle high bit? */
  unsigned vc_decscnm        : 1;  /* Screen Mode */
  unsigned vc_decom          : 1;  /* Origin Mode */
  unsigned vc_decawm         : 1;  /* Autowrap Mode */
  unsigned vc_deccm          : 1;  /* Cursor Visible */
  unsigned vc_decim          : 1;  /* Insert Mode */
  unsigned vc_deccolm        : 1;  /* 80/132 Column Mode */

  /* misc */
  unsigned vc_ques           : 1;
  unsigned vc_can_do_color   : 1;
  unsigned vc_report_mouse   : 2;
  unsigned vc_utf            : 1;  /* Unicode UTF-8 encoding */
  unsigned char vc_utf_count;
  unsigned vc_utf_char;
  unsigned vc_tab_stop[8];           /* Tab stops. 256 columns. */
  unsigned char vc_palette[16*3];    /* Colour palette for VGA+ */
  unsigned short * vc_translate;
  unsigned char vc_G0_charset;
  unsigned char vc_G1_charset;
  unsigned char vc_saved_G0;
  unsigned char vc_saved_G1;
  unsigned vc_resize_user;     /* resize request from user */
  unsigned vc_bell_pitch;      /* Console bell pitch */
  unsigned vc_bell_duration;   /* Console bell duration */

  unsigned vc_s_ensity       : 2;  /* saved rendition */
  unsigned vc_s_italic       : 1;
  unsigned vc_s_underline    : 1;
  unsigned vc_s_blink        : 1;
  unsigned vc_s_reverse      : 1;
  unsigned char :0;

  unsigned char vc_need_wrap : 1;
#endif




struct video_att {
  /* attribute flags */
  unsigned bold      : 1;
  unsigned underline : 1;
  unsigned reverse   : 1;
  unsigned bg        : 3;
  unsigned fg        : 3;
};

#define NPARS        16
#define VT100_MODE   1
#define MAXSIZEX     132
#define MAXSIZEY     30


struct term {
  u8 nrows;
  u8 ncols;
  u8 state;
  u8 offset;
  u8 mode;
  u8 question;

  u8 pars[NPARS];
  u8 npars;

  u8 xpos, ypos, xpos_s, ypos_s;
  struct video_att video, video_s;
  u8 bg_color;
  u8 fg_color;

  u8 top;
  u8 botton;

  u16 ** map_char;
  u16 map_char_buf[MAXSIZEY * MAXSIZEX];
};



void init_term(void);
void term_write (u16 * buf, int count);
#endif /* _VT100_H_ */
