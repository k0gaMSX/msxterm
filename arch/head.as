;;; -*- mode: asm ; coding: iso-8859-1 -*-

        psect   text,global,pure,class=CODE
        psect   strings,global,pure,class=CODE
        psect   const,global,pure,class=CODE
        psect   data,global,class=DATA
        psect   bss,global,class=DATA

        global  _main
        global  start, __Lbss, __Hbss, _stack_init
        global  small_model

small_model     equ     1

        psect   text
start:
        di
        ld      de,__Lbss       ;clear bbs psect
        ld      hl,__Hbss
        or      a
        sbc     hl,de
        ld      c,l
        ld      b,h
        dec     bc               ;bc = counter - 1

        ld      h,d
        ld      l,e
        inc     de
        ld      (hl),0            ;can fail if bc = 0
        ld      a,b
        or      c
        jr      z,1f
        ldir

1:      im      0                 ;set im 0
        ld      sp,(_stack_init)  ;set initial stack

        ld      hl,038h
        ld      de,isr
        ld      (hl),0c3h
        inc     hl
        ld      (hl),e
        inc     hl
        ld      (hl),d            ;set interrupt service routine
        ei
        jp      _main             ;never return


isr:
        push    af
        in      a,(99h)
        pop     af
        ei
        ret
        end     start
