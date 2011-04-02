;	word relational operation - returns flags as though
;	a subtract was done.

	psect	text,class=CODE
	global	wrelopbc

wrelopbc:
	ld	a,h		;test for differing signs
	xor	b
	jp	p,2f		;the same, so ok
	ld	a,b		;get the sign of the LHS
	or	2		;ensure zero flag is reset, set sign flag
	ld	a,h		;get RHS
	rla			;rotate hi bit into carry - Z and S unchanged
	ret			;done
2:
	ld	a,b
	sub	h		;set the flags
	jr	nz,1f		;go return if not zero
	ld	a,c
	sub	l
	ret	z		;if zero, we're done
1:
	ld	a,2		;make non-zero
	rra			;rotate	carry into sign
	or	a		;set minus flag
	rlca			;put carry flag	back
	ret
