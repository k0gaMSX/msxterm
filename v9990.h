
#ifndef V9990_H_
#define V9990_H_
#include "types.h"


#define V9990_VRAM_PORT             0x60
#define V9990_PALETTE_PORT          0x61
#define V9990_COMMAND_PORT          0x62
#define V9990_DATA_PORT             0x63
#define V9990_SELECT_PORT           0x64
#define V9990_STATUS_PORT           0x65
#define V9990_INT_PORT              0x66
#define V9990_CONTROL_PORT          0x67
#define V9990_OUTPUT_PORT           0x6f



extern void setpal(const uint8_t * pal);
extern void vdp_init(void);
extern void setspd(void);
extern void resetspd(void);
extern void set_cursor_pos(unsigned x, unsigned y);
extern void ldirvm(void * src, unsigned cnt);
extern void ldirmv(void * dst, unsigned cnt);
extern void vwrite_ad(uint16_t vpos, int8_t page);
extern void vread_ad(uint16_t vpos, int8_t page);

#endif /* V9990_H_ */
