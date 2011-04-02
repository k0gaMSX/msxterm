;;; -*- mode: asm -*-

	global	_memset
	signat	_memset,12346
	psect	text,class=CODE
;
;	On entry, de has s (the buffer), bc has c (the character)
;	and n (the count) is on the stack.

_memset:
	pop	hl		;return address
	ex	(sp),hl		;get n into l
	push	hl
	ld	l,c
	pop	bc		;n now in bc
	push	de
	ld	a,b		;check count
	or	a
	jr	nz,1f
	or	c
	jr	z,done
1:
	dec	bc		;decrement count
	ld	a,b
	or	c
	ld	a,l
	ld	(de),a
	jr	z,done		;if count now zero
	ld	l,e
	ld	h,d
	inc	de
	ldir
done:
	pop	hl
	ret

	end
