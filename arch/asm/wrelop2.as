;	word relational operation - returns flags as though
;	a subtract was done.

	psect	text,class=CODE
	global	wrelopde

wrelopde:
	ld	a,h		;test for differing signs
	xor	d
	jp	p,2f		;the same, so ok
	ld	a,d		;get the sign of the LHS
	or	2		;ensure zero flag is reset, set sign flag
	ld	a,h		;get RHS
	rla			;rotate hi bit into carry - Z and S unchanged
	ret			;done
2:
	ld	a,d
	sub	h		;set the flags
	jr	nz,1f		;go return if not zero
	ld	a,e
	sub	l
	ret	z		;if zero, we're done
1:
	ld	a,2		;make non-zero
	rra			;rotate	carry into sign
	or	a		;set minus flag
	rlca			;put carry flag	back
	ret
