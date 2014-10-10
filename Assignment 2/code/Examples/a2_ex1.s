.pos 0x100

    # b = a + 5
    ld $a, r0           # r0 = address of a
    ld 0(r0), r0        # r0 = (value of) a
    ld $5, r1           # r1 = 5
    add r1, r0          # r0 = a + 5
    ld $b, r1           # r1 = address of b
    st r0, 0(r1)        # b = a + 5

    # c[0] = ~(a + c[b])
    ld $a, r0
    ld 0(r0), r0        # r0 = a
    ld $c, r1           # r1 = address of c
    ld $b, r2			# r2 = address of b
    ld 0(r2), r2        # r2 = b
    ld (r1,r2,4), r2    # r2 = c[b]
    add r2, r0          # r0 = a + c[b]
    not r0              # r0 = ~(a + c[b])
    st r0, 0(r1)        # c[0] = r0 = ~(a + c[b])

    halt

.pos 0x200
# Data area

a:  .long 0             # a
b:  .long 0             # b
c:  .long 0             # c[0]
    .long 0             # c[1]
    .long 0             # c[2]
    .long 0             # c[3]
