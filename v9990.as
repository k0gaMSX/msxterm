; -*- mode: asm -*-

VRAM_PORT            equ 060h
PALETTE_PORT         equ 061h
COMMAND_PORT         equ 062h
DATA_PORT            equ 063h
SELECT_PORT          equ 064h
STATUS_PORT          equ 065h
INT_PORT             equ 066h
CONTROL_PORT         equ 067h
OUTPUT_PORT          equ 06fh


OPCODE_STOP          equ 000h
OPCODE_LMMC          equ 010h
OPCODE_LMMV          equ 020h
OPCODE_LMCM          equ 030h
OPCODE_LMMM          equ 040h
OPCODE_CMMC          equ 050h
OPCODE_CMMK          equ 060h
OPCODE_CMMM          equ 070h
OPCODE_BMXL          equ 080h
OPCODE_BMLX          equ 090h
OPCODE_BMLL          equ 0A0h
OPCODE_LINE          equ 0B0h
OPCODE_SRCH          equ 0C0h
OPCODE_POINT         equ 0D0h
OPCODE_PSET          equ 0E0h
OPCODE_ADVN          equ 0F0h



VRAM_WRITE_REG       equ 0
VRAM_READ_REG        equ 3
SCRMODE0_REG         equ 6
SCRMODE1_REG         equ 7
CONTROL_REG          equ 8
PALETTE_CTRL_REG     equ 13
PALETTE_PTR_REG      equ 14
BACKDROP_REG         equ 15
ADJUST_REG           equ 16
CURSOR_OFFSET_REG    equ 28
DS_X_REG             equ 36
OPCODE_REG           equ 52



CURSOR_SIZE       equ     080h


       global  small_model
       psect   text,class=CODE





;;; _vread_ad: Store memory page of vram for reading
;;; input: de <- address memory
;;;        c <- page

        psect   text
        global  _vread_ad
        signat  _vread_ad,8248
_vread_ad:
        ld      a,VRAM_READ_REG
        jr      memory_pointer


;;; _vwrite_ad: Store memory page of vram for writing
;;; input: de <- address memory
;;;        c <- page

        psect   text

        global  _vwrite_ad
        signat  _vwrite_ad,8248

_vwrite_ad:
        ld      a,VRAM_WRITE_REG

memory_pointer:
        di
        out     (SELECT_PORT),a
        ld      a,e
        out     (DATA_PORT),a
        ld      a,d
        out     (DATA_PORT),a
        ld      a,c
        out     (DATA_PORT),a
        ret


;;; _ldirmv: read a memory block from vram. Vram memory must be set
;;; before calling this function
;;; input: de <- destiny memory address
;;;        bc <- count

        psect   text
        global  _ldirmv
        signat  _ldirmv,8248

_ldirmv:
        ex      de,hl
        xor     a
        or      b
        jp      nz,1f
        inc     b

1:      push    bc
        ld      b,c
        ld      c,VRAM_PORT
        inir
        pop     bc
        ld      c,0
        djnz    1b
        ret


;;; _ldirvm: write a memory block to vram. Vram memory must be set
;;; before calling this function
;;; input: de <- source memory address
;;;        bc <- count

        psect   text
        global  _ldirvm
        signat  _ldirvm,8248

_ldirvm:
        ex      de,hl
        xor     a
        or      b
        jp      nz,1f
        inc     b

1:      push    bc
        ld      b,c
        ld      c,VRAM_PORT
        otir
        pop     bc
        ld      c,0
        djnz    1b
        ret



;;; _lmmv: Logical move to memoory from CPU
;;; input: de <- command data for LMMV
       psect   text
lmmv:
       di
       ld a,DS_X_REG
       out (SELECT_PORT),a
       ld bc,011h*256 + DATA_PORT
       otir
       ret


;;; _reset:  Reset the v9990 chip to a know state

        psect   text
reset:
       di
       ld      a,ADJUST_REG         ;Save adjust value
       out     (SELECT_PORT),a
       in      a,(DATA_PORT)
       ld      b,a

       ld      a,02h               ;set reset state
       out     (CONTROL_PORT),a
       xor     a                   ;clear reset state
       out     (CONTROL_PORT),a

       ld      a,ADJUST_REG        ;restore adjust value
       out     (SELECT_PORT),a
       ld      a,b
       out     (DATA_PORT),a

       ld      a,OPCODE_REG        ;stop all commands
       out     (SELECT_PORT),a
       ld      a,OPCODE_STOP
       out     (DATA_PORT),a

       ld      a,CONTROL_REG          ;Display screen, disable sprites
       out     (SELECT_PORT),a
       ld      a,0c2h
       out     (DATA_PORT),a

       ld      a,PALETTE_PTR_REG  ;Clean pallete
       out     (SELECT_PORT),a
       xor     a
       out     (DATA_PORT),a

       ld      b,64
       xor     a
1:
       out     (PALETTE_PORT),a
       out     (PALETTE_PORT),a
       out     (PALETTE_PORT),a
       djnz    1b

       out     (06fh),a
       ret



;;; R#5 and R#6
;;; DSPM	Display mode selection
;;; 3	: Stand-by mode (non-display, no VRAM refresh, kanji ROM readable)
;;; 2	: Bit map mode (Bl-6)
;;; 1	: P2 mode
;;; 0	: P1 mode
;;;
;;; DCKM	Pixel clock selection (used in combination with MCS of P#7)
;;;        MCS=0	        MCS=1
;;; 2	: XTAL1	        MCKIN
;;; 1	: XTAL1/2	MCKIN/2
;;; 0	: XTAL1/4	->MCKIN/4	(Number of division of master clock)
;;; -> Undocumented setting
;;;
;;; XIMM	Selection of number of pixels in X direction of image space (Number
;;;      of pixels in Y direction is automatically calculated from XIMM and CLRM)
;;; 3	: 2048 pixels
;;; 2	: 1024 pixels
;;; 1	: 512 pixels
;;; 0	: 256 pixels
;;;
;;; CLRM	Selection of bit number per pixel
;;; 3	: 16 bits/pixel
;;; 2	: 8 bits/pixel
;;; 1	: 4 bits/pixel
;;; 0	: 2 bits/pixel
;;;
;;; C25M	Selection of 640X480 mode, valid when HSCN is "1".
;;; 1	: B6 mode
;;; 0	: other modes
;;;
;;; SM1	Selection of total number of vertical lines during non-interlace, NTSC
;;; 1	: 263 lines (In combination with SM, the sub-carrier phase is
;;;        inverted  for each frame.)
;;; 0	: 262 lines
;;;
;;; SM	Selection of horizontal frequency (invalid when in B5 and B6 modes)
;;; 1	: 1H=fsc/227.5 (the sub-carrier phase is inverted for each line.)
;;; 0	: 1H=fsc/228
;;;
;;; PAL	Selection of PAL mode (invalid when in B5 and B6 modes)
;;; 1	: Vertical frequency becomes PAL specification.
;;; 0	: Vertical frequency becomes NTSC specification.
;;;
;;; EO	Selection of vertical resolution during interlace (invalid when in
;;;      B5  and B6 modes)
;;; 1	: Twice the vertical resolution during non-interlace
;;; 0	: Same as during non-interlace
;;;
;;; IL	Selection of interlace mode (invalid when in B5 and B6 modes)
;;; 1	: Interlace
;;; 0	: Non-interlace
;;;
;;; HSCN	Selection of horizontal high scan mode
;;; 1	: B5, B6 modes
;;; 0	: other modes
;;;
;;; R#13 description
;;; PLTM	Selection of color palette mode
;;; 3	: YUV mode
;;; 2	: YJK mode
;;; 1	: 256 color mode
;;; 0	: Palette mode
;;;
;;; YAE	Selection of YJK (YUV) and RGB mixing mode (valid when in YUV and
;;;      YJK modes)
;;; 1	: YJK (YUV) and RGB images are displayed together.
;;; 0	: Only YJK (YUV) image is displayed.
;;;
;;; PLTAIH  Palette No. increment control at color palette read-out
;;; 1	: R#14 palette pointer is not changed by P#1 read-out.
;;; 0	: After P#1 read-out, R#14 palette pointer undergoes an increment.
;;;
;;; PLTO5-2	: Still screen palette No. offset

;;; Setting B4 mode

        psect   text
setmode:
        di
        ld      a,SCRMODE0_REG     ;DSPM = 2,  DCKM = 2, XIMM = 2, CLRM = 1
        out     (SELECT_PORT),a
        ld      a,0a9h
        out     (DATA_PORT),a

        ld      a,SCRMODE1_REG     ;C25M   = SM1 = SM = PAL = EO = IL = HSCN = 0
        out     (SELECT_PORT),a
        xor     a
        out     (DATA_PORT),a

        ld      a,01h              ;MCS = 1
        out     (CONTROL_PORT),a

        ld      a,PALETTE_CTRL_REG ;PLT5-0 = PLTM = PLTAIH = YAE = PLTM = 0
        out     (SELECT_PORT),a
        xor     a
        out     (DATA_PORT),a

        ld      a,BACKDROP_REG     ;BACKDROP_COLOR = 0
        out     (SELECT_PORT),a
        xor     a
        out     (DATA_PORT),a
        ret


;;; setcolors:  Set a number of colors into the pallete
;;; input:  hl <- pointer to the colors in db R,G,B format
;;;         b  <- Number of colors
;;;         c  <- offset into the pallete

        psect   text
setcolors:
        di
        ld       a,PALETTE_PTR_REG      ;set pallete pointer
        out      (SELECT_PORT), a
        ld       a,c
        rlca
        rlca
        out      (DATA_PORT),a

        ld       a,b                     ;write the data
        add      a,a
        add      a,b
        ld       b,a
        ld       c,PALETTE_PORT
        otir
        ret


;;; _setpal: Sets a 16 colors pallete
;;; Input: de <- pointer to the pallete

        psect   text
        global  _setpal
        signat  _setpal,4152
_setpal:
        push    de
        push    de
        push    de

        ld      l,e
        ld      h,d
        ld      c,0
        ld      b,16
        call    setcolors       ;first 16 colors

        ld      c,16
        ld      b,16
        pop     hl
        call    setcolors       ;second 16 colors

        ld      c,32
        ld      b,16
        pop     hl
        call    setcolors       ;third 16 colors

        ld      c,48
        ld      b,16
        pop     hl
        jp      setcolors       ;fourth 16 colors


;;; set_cursor_pattern: Write the cursor pattern into the vram
;;; Input:  de <- Pointer to pattern data
        psect   text
set_cursor_pattern:
        di
        ld      a,VRAM_WRITE_REG        ;writing in 007ff00h VRAM address
        out     (SELECT_PORT),a
        xor     a
        out     (DATA_PORT),a
        ld      a,0ffh
        out     (DATA_PORT),a
        ld      a,7
        out     (DATA_PORT),a

        ld      bc,080h*256 + VRAM_PORT
        otir
        ret


;;; _set_cursor_pos: Writte cursor coordenates into the vram
;;; Input: bc <- y pos
;;;        de <- x pos

        psect   text
        global  _set_cursor_pos
        signat  _set_cursor_pos,8248
_set_cursor_pos:
        di
        ld      a,VRAM_WRITE_REG        ;writing in 007fe00h VRAM address
        out     (SELECT_PORT),a
        xor     a
        out     (DATA_PORT),a
        ld      a,0feh
        out     (DATA_PORT),a
        ld      a,7
        out     (DATA_PORT),a

        ld      l,c
        ld      c,VRAM_PORT

        out     (c),l           ;lower y
        out     (c),l           ;dummy byte
        out     (c),b           ;upper y
        out     (c),b           ;dummy byte

        out     (c),e           ;lower x
        out     (c),e           ;dummy byte
        ld      a,d
        and     3
        or      0e0h
        out     (c),a           ;upper x + att
        ret

;;; Set the Cursor Sprite Pallete Offset (CSPO)
;;; Input a <- Offset of the cursor pallete

        psect   text
setcspo:
        di
        push    af
        ld      a,CURSOR_OFFSET_REG
        out     (SELECT_PORT),a
        pop     af
        out     (DATA_PORT),a
        ret



        psect   text
        global  _setspd
        signat  _setspd,24
_setspd:
        di
        ld      a,CONTROL_REG + 0c0h
        out     (SELECT_PORT),a
        in      a,(DATA_PORT)
        and     0bfh             ;reset b6 (SPD bit)
        out     (DATA_PORT),a
        ret



        psect   text
        global  _resetspd
        signat  _resetspd,24
_resetspd:
        di
        ld      a,CONTROL_REG + 0c0h
        out     (SELECT_PORT),a
        in      a,(DATA_PORT)
        or      040h             ;set b6 (SPD bit)
        out     (DATA_PORT),a
        ret

        psect   text
        global  _waitce
        signat  _waitce,24
_waitce:
        in       a,(STATUS_PORT)
        rrca
        ret      nc
        jr       _waitce

;;;
;;;  Reset v9990 chip to a know state, set B4 mode, load default pallete, load
;;;  cursor pattern and set initial cursor coordenate


        psect   const,class=CODE
        psect   const
pallete:
        db    0,0,0,    31,0,0,    0,31,0,    31,31,0
        db    0,0,31,   31,0,31,   0,31,31,   16,16,16
        db    31,31,31, 31,31,31,  31,31,31,  31,31,31
        db    31,31,31, 31,31,31,  31,31,31,  31,31,31

cursor:
        db   0ffh,0,0,0
        db   0ffh,0,0,0
        db   0ffh,0,0,0
        db   0ffh,0,0,0
        db   0ffh,0,0,0
        db   0ffh,0,0,0
        db   0ffh,0,0,0
        db   0ffh,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0
        db   0,0,0,0


        psect   text
        global  _vdp_init
        signat  _vdp_init,24
_vdp_init:
        call    reset           ;reset the chip
        call    setmode

        ld      de,pallete      ;set default pallete
        call    _setpal
        call    _setspd         ;enable sprites
        ld      a,1
        call    setcspo         ;set upper part of cursor

        ld      hl,cursor
        call    set_cursor_pattern ;define cursor pattern

        ld      de,0            ;set the cursor in initial position
        ld      bc,0
        call    _set_cursor_pos
        ei
        ret
