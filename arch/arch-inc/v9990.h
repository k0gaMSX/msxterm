
#ifndef V9990_H_
#define V9990_H_
#include "arch/types.h"


#define V9990_VRAM_PORT             0x60
#define V9990_PALETTE_PORT          0x61
#define V9990_COMMAND_PORT          0x62
#define V9990_DATA_PORT             0x63
#define V9990_SELECT_PORT           0x64
#define V9990_STATUS_PORT           0x65
#define V9990_INT_PORT              0x66
#define V9990_CONTROL_PORT          0x67
#define V9990_OUTPUT_PORT           0x6f



/* -------------   VDP  COMMAND ------------*/
/*  Command being excuted is stopped  */
#define V9990_OP_STOP   0x00
/*  Data is transferred from CPU to VRAM rectangle area */
#define V9990_OP_LMMC   0x10
/*  VRAM rectangle area is painted out */
#define V9990_OP_LMMV   0x20
/*  VRAM rectangle area is transferred to CPU */
#define V9990_OP_LMCM   0x30
/*  Rectangle area os transferred from VRAM to VRAM */
#define V9990_OP_LMMM   0x40
/*CPU character data is color-developed and transferred to VRAM rectangle area*/
#define V9990_OP_CMMC   0x50
/*  Kanji ROM data is is color-developed & transferred to VRAM rectangle area */
#define V9990_OP_CMMK   0x60
/*  VRAM character is color-developed & transferred to VRAM rectangle area  */
#define V9990_OP_CMMM   0x70
/*  Data on VRAM linear address is transferred to VRAM rectangle area */
#define V9990_OP_BMXL   0x80
/*  VRAM rectangle area is transferred to VRAM linear address  */
#define V9990_OP_BMLX   0x90
/*  Data on VRAM linear address is transferred to VRAM linear address  */
#define V9990_OP_BMLL   0xA0
/*  Straight line is drawer on X-Y co-ordinates */
#define V9990_OP_LINE   0xB0
/*  Border color co-ordinates on X-Y are detected */
#define V9990_OP_SRCH   0xC0
/*  Color code on specified point on X-Y is read out */
#define V9990_OP_POINT  0xD0
/*  Drawing is executed at drawing point on X-Y co-ordinates */
#define V9990_OP_PSET   0xE0
/*  Drawing point on X-Y co-ordinates is shifted */
#define V9990_OP_ADVN   0xF0

/* -------------  VDP LOGIC OPERATIONS -------------  */
#define V9990_LOGIC_IMP  0x0c
#define V9990_LOGIC_NEG  0x03
#define V9990_LOGIC_AND  0x08
#define V9990_LOGIC_OR   0x0e
#define V9990_LOGIC_EOR  0x06

/**
 * we define this struct because we can be sure there isn't padding. we
 * are too much near of the hardware, so don't be portable isn't a problem.
 **/


struct LMMVpars {
  uint32_t dummy;                /* dummy*/
  uint16_t dx, dy;               /* destiny coordenates */
  uint16_t nx, ny;               /* number of pixels */
  uint8_t arg;                   /* command argument */
  uint8_t op_logic;              /* Logic operation */
  uint16_t mask;                 /* write mask */
  uint16_t fg, bg;               /* font color */
  uint8_t op;                    /* command operation */
};




struct CMMMpars {
  uint8_t sa_off1, dummy;         /* address*/
  uint8_t sa_off2, sa_page;
  uint16_t dx, dy;               /* destiny coordenates */
  uint16_t nx, ny;               /* number of pixels */
  uint8_t arg;                   /* command argument */
  uint8_t op_logic;              /* Logic operation */
  uint16_t mask;                 /* write mask */
  uint16_t fg, bg;               /* font color */
  uint8_t op;                    /* command operation */
};


struct LMMMpars {
  uint16_t sx, sy, dx, dy;       /* coordenates */
  uint16_t nx, ny;               /* number of pixels */
  uint8_t arg;                   /* command argument */
  uint8_t op_logic;              /* Logic operation */
  uint16_t mask;                 /* write mask */
  uint16_t fg, bg;               /* font color */
  uint8_t op;                    /* command operation */
};


extern void setpal(const uint8_t * pal);
extern void v9990_init(void);
extern void setspd(void);
extern void resetspd(void);
extern void set_cursor_pos(unsigned x, unsigned y);
extern void ldirvm(void * src, uint8_t cnt);
extern void ldirmv(void * dst, uint8_t cnt);
extern void vwrite_ad(uint32_t addr);
extern void vread_ad(uint32_t addr);
extern void waitce(void);
extern void waittr(void);
extern void v9990cmd(void *cmd);
extern void videomode(void);
extern unsigned char v9990_handler(void *data);

#define NUMBER_OF_PARS 21
#endif /* V9990_H_ */
