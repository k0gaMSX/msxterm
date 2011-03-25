; -*- mode: asm -*-



KBD_NUMBER_ROW          equ     11
KBD_REPEAT_TIME         equ     10


        psect   bss,class=DATA
        psect   bss
lastkeycode:    ds      1
lasttime:       ds      1
kbd_matrix:     ds      KBD_NUMBER_ROW



        psect   text,class=CODE
        psect   text
        global  small_model
        global  _kbd_hook
        global  _kbd_event
        signat  _kbd_event,4152
        signat  _kbd_hook,24
;;; Interrupt hook. Scan keyboard matrix and raise kdb

_kbd_hook:
        ld      hl,kbd_matrix
        in      a,(0aah)        ;save 0aah value
        and     0f0h
        ld      c,a
        ld      e,0             ;keycode = 0
        ld      b,e

1:      ld      a,c             ;select the row
        or      b
        out     (0aah),a
        in      a,(0a9h)         ;read the row
        call    scan_row        ;scan it!
        inc     b
        ld      a,KBD_NUMBER_ROW
        cp      b
        jp      nz,1b

        ld      hl,lasttime     ;test repeat time
        ld      a,KBD_REPEAT_TIME
        cp      (hl)
        jp      nz,no_repeat

        ld      a,(lastkeycode)
        call    new_key
        ret

no_repeat:
        inc    (hl)
        ret




;;; _scan_row: scan each bit of the row and raise the event
;;; input: e <- initial keycode
;;;        a <- row content
;;;        hl <- pointer to shadow row
;;; Used register in caller routine:
;;;        b <- loop counter
;;;        c <- aa original content


scan_row:
        ld      d,a             ;save the row
        xor     (hl)
        ld      (hl),d          ;store row into the matrix
        inc     hl
        or      a               ;test xor bits
        jp      nz,scanbits

        ld      a,8             ;there isn't changed bits
        add     a,e             ;so we only have to increment
        ld      e,a             ;the keycode
        ret

scanbits:
        push    bc              ;save caller register
        ld      d,1             ;initial mask
        ld      b,a             ;store the xor in b

scanbit:
        ld      a,b
        and     d               ;test a bit
        call    nz,new_key      ;raise the event
        inc     e               ;next opcode
        sla     d
        jp      nc,scanbit

        pop     bc
        ret


;;; a <- keycode

new_key:
        ld      (lastkeycode),a
        xor     a
        ld      (lasttime),a
        jp      _kbd_event



