
#ifndef _VT100_H_
#define _VT100_H_



#include "arch/types.h"
#include "arch/vram.h"

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




#define NPARS        16
#define VT100_MODE   1
#define MAXSIZEX     132
#define MAXSIZEY     30


struct term {
     unsigned char nrows, ncols;
     unsigned char state;
     unsigned char offset;
     unsigned char mode;
     unsigned char question;

     unsigned char pars[NPARS];
     unsigned char npars;

     unsigned char need_wrap;
     unsigned char decawm;      /* Autowrap Mode */
     unsigned char decom;       /* origin mode */

     unsigned char xpos, ypos, xpos_s, ypos_s;
     struct video_att video, video_s;
     unsigned char bg_color;
     unsigned char fg_color;

     unsigned char top;
     unsigned char botton;
};


extern void con_write (const void *buf, unsigned count);
extern void con_init(void);
#endif /* _VT100_H_ */
