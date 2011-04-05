; -*- mode: asm -*-



KBD_NUMBER_ROW          equ     11 ;Number of row of the keyboard matrix
KBD_REPEAT_TIME         equ     50 ;time before we began to repeat a key
KBD_REPEAT_RATE         equ      5 ;time between each repeat after the initial time

        psect   data,class=DATA
lastkeycode:    db      080h    ;it is necessary to avoid repeat in the beginning


        psect   bss,class=DATA
        psect   bss
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
        cpl
        call    scan_row        ;scan it!
        inc     b
        ld      a,KBD_NUMBER_ROW
        cp      b
        jp      nz,1b

        ld      a,(lastkeycode)
        and     80h
        ret     nz              ;it was a release, so there isn't repeat

        ld      hl,lasttime     ;test repeat time
        or      (hl)
        jp      nz,no_repeat

        ld      a,(lastkeycode)
        ld      e,a
        call    new_key
        ld      a,KBD_REPEAT_RATE
        ld      (lasttime),a
        ret

no_repeat:
        dec    (hl)
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
;;; d <- scan row
;;; e <- keycode
;;; a <- xor

        ld      c,1             ;initial mask
        ld      b,a             ;store the xor in b

scanbit:
        rrc     d               ;load the row bit in b7 of d
        ld      a,b
        and     c               ;test a bit
        jp      z,1f

        ld      a,d
        cpl
        and     80h
        or      e               ;set b7 of e = 1 if key release

        call    new_key       ;raise the event
1:      inc     e             ;next keycode
        sla     c
        ld      a,c           ; c = 0 means we had tested all bits
        or      a
        jp      nz,scanbit

        pop     bc
        ret


;;; a <- keycode + push

new_key:
        push    hl
        push    de
        push    bc
        ld      e,a
        ld      (lastkeycode),a
        ld      a,KBD_REPEAT_TIME
        ld      (lasttime),a
        call      _kbd_event
        pop     bc
        pop     de
        pop     hl
        ret

