;;; -*- mode: asm ; coding: iso-8859-1 -*-
        psect   text,class=CODE
        global  _v9958_init
        signat  _v9958_init,24

_v9958_init:
        xor     a               ;disable interrupt vertical and
        out     (99h),a         ;disable visualization
        ld      a,128+1
        out     (99h),a
        ret


