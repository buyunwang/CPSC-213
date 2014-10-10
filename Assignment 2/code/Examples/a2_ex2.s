.pos 0x100

    # d[a & 7]
    ld $a, r0           # r0 = address of a
    ld 0(r0), r0        # r0 = a
    ld $7, r1           # r1 = 7
    and r1, r0          # r0 = a & 7
    ld $d, r1           # r1 = address of d
    ld (r1,r0,4), r0    # r0 = d[a & 7]

    # b[c & 3]
    ld $c, r1           # r1 = address of c
    ld 0(r1), r1        # r1 = c
    ld $3, r2           # r2 = 3
    and r2, r1          # r1 = c & 3
    ld $b, r2           # r2 = address of b
    ld (r2,r1,4), r1    # r1 = b[c & 3]

    # (d[a & 7] + b[c & 3])&42
    add r1, r0          # r0 = d[a & 7] + b[c & 3]
    ld $42, r1          # r1 = 42
    and r1, r0          # r0 = (d[a & 7] + b[c & 3])&42

    # b[0] + ...
    ld 0(r2), r1        # r1 = b[0]
    add r1, r0          # r0 = b[0] + ...

    # ... + 1
    inc r0              # r0 = (b[0] + ... ) + 1

    # c = ...
    ld $c, r1           # r1 = address of c
    st r0, 0(r1)        # c = ...

    halt

.pos 0x200
# Data area

a:  .long 0             # a
b:  .long 0             # b[0]
    .long 0             # b[1]
    .long 0             # b[2]
    .long 0             # b[3]
c:  .long 0             # c
d:  .long 0             # d[0]
    .long 0             # d[1]
    .long 0             # d[2]
    .long 0             # d[3]
    .long 0             # d[4]
    .long 0             # d[5]
    .long 0             # d[6]
    .long 0             # d[7]
