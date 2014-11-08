.pos 0x0
    ld   $l4, r2
    ld   0(r0), r1
    ld   (r0, r0, 4), r1
    st   r1, 0(r0)
    st   r1, (r0, r0, 4)
    mov  r0, r1
    add  r0, r1
    and  r0, r1
    inc  r1
    inca r1
    dec  r1
    deca r1
    not  r1
    gpc  $0, r1
    shr  $0, r1
    br   l0
l0: beq  r0, l1
l1: bgt  r0, l2
l2: j    l3
l3: j    0(r2)
l4: halt
