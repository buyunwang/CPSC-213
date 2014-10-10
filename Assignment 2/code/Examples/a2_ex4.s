.pos 0x100

    # STATEMENT 1 : c[b] = ((c[a] + 1) >> 15) - 1;
    ld $c, r0           # r0 = address of c
    ld $a, r1           # r1 = address of a
    ld 0(r1), r1        # r1 = a
    ld (r0,r1,4), r1    # r1 = c[a]
    inc r1              # r1 = c[a] + 1
    shr $15, r1         # r1 = (c[a] + 1) >> 15
    dec r1              # r1 = ((c[a] + 1) >> 15) - 1
    ld $b, r2           # r2 = address of b
    ld 0(r2), r2        # r2 = b
    st r1, (r0,r2,4)    # c[b] = ((c[a] + 1) >> 15) - 1

    # STATEMENT 2 : b = b + 4
    ld $b, r0           # r0 = address of b
    ld 0(r0), r1        # r1 = b
    inca r1             # r1 = b + 4
    st r1, 0(r0)        # b = r1 = b + 4

    # STATEMENT 3 : d[b] = d[a] - 4
    ld $d, r0           # r0 = address of d
    ld 0(r0), r0        # r0 = d (which is itself an address)
    ld $a, r1           # r1 = address of a
    ld 0(r1), r1        # r1 = a
    ld (r0,r1,4), r1    # r1 = d[a]
    deca r1             # r1 = d[a] - 4
    ld $b, r2           # r2 = address of b
    ld 0(r2), r2        # r2 = b
    st r1, (r0,r2,4)    # d[b] = d[a] - 4

    halt

.pos 0x200
# Data area

a:  .long 0             # a
b:  .long 0             # b
c:  .long 0             # c[0]
    .long 0             # c[1]
    .long 0             # c[2]
    .long 0             # c[3]
    .long 0             # c[4]
    .long 0             # c[5]
    .long 0             # c[6]
    .long 0             # c[7]
d:  .long c             # d

