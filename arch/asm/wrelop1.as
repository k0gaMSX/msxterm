;	word relational	operation - returns flags correctly for
;	comparision of words in hl and bc

	psect	text,class=CODE
	global	wrelop1

wrelop1:
	ld	a,h
	xor	b		;compare signs
	jp	m,1f		;if different, return sign of lhs
	sbc	hl,bc		;just set flags as normal
	ret
1:
	ld	a,h		;get sign of lhs
	and	80h		;mask out sign flag
	sbc	hl,bc		;set carry flag if appropriate
	inc	a		;set sign flag as appropriate and reset Z flag
	ret
