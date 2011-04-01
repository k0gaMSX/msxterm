;;; -*- mode: asm ; coding: iso-8859-1 -*-
stack_start     equ     08000h


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

        jp      _main             ;never return
        psect   text
        end     start
