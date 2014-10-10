.pos 0x100

    # STATEMENT 1 : d = d + (a[d & 3] & b[(d+1) & 3]) + ~c[(d+2) & 3];
    # Retrieve d
    ld $d, r0           # r0 = address of d
    ld 0(r0), r0        # r0 = d
    
    # Compute d + 1 and d + 2
    mov r0, r1          # r1 = d
    mov r1, r2          # r2 = d
    inc r2              # r2 = d + 1
    mov r2, r3          # r3 = d + 1
    inc r3              # r3 = d + 2

    # Compute d&3, (d+1)&3, and (d+2)&3
    ld $3, r4           # r4 = 3
    and r4, r1          # r1 = d & 3
    and r4, r2          # r2 = (d + 1) & 3
    and r4, r3          # r3 = (d + 2) & 3

    # a[d & 3] & b[(d+1)&3]
    ld $a, r4           # r4 = address of a
    ld (r4,r1,4), r1    # r1 = a[d & 3]
    ld $b, r4           # r4 = address of b
    ld (r4,r2,4), r2    # r2 = b[(d+1) & 3]
    and r2, r1          # r1 = a[d & 3] & b[(d+1)&3]

    # ... + ~c[(d+2) & 3]
    ld $c, r4           # r4 = address of c
    ld (r4,r3,4), r3    # r3 = c[(d+2) & 3]
    not r3              # r3 = ~c[(d+2) & 3]
    add r3, r1          # r1 = (a[d & 3] & b[(d+1)&3]) + ~c[(d+2) & 3]

    # d = d + ...
    add r1, r0          # r0 = d + ...
    ld $d, r1           # r1 = address of d
    st r0, 0(r1)        # d = d + ...

    # STATEMENT 2 : a[0] = b[1] >> 2;
    ld $b, r0           # r0 = address of b
    ld 4(r0), r0        # r0 = b[1]
    shr $2, r0          # r0 = b[1] >> 2
    ld $a, r1           # r1 = address of a
    st r0, 0(r1)        # a[0] = r0 = b[1] >> 2

    # STATEMENT 3 : b[1] = c[2] << 2;
    ld $c, r0           # r0 = address of c
    ld 8(r0), r0        # r0 = c[2]
    shl $2, r0          # r0 = c[2] << 2
    ld $b, r1           # r1 = address of b
    st r0, 4(r1)        # b[1] = r0 = c[2] << 2

    # STATEMENT 4 : c[d & 3] = a[(d+1) & 3]
    # Retrieve and compute d and (d+1)&3
    ld $d, r0           # r0 = address of d
    ld 0(r0), r0        # r0 = d
    mov r0, r1          # r1 = d
    inc r1              # r1 = d + 1
    ld $3, r2           # r2 = 3
    and r2, r0          # r0 = d & 3
    and r2, r1          # r1 = (d+1) & 3

    # c[d & 3] = a[(d+1) & 3]
    ld $a, r2           # r2 = address of a
    ld $c, r3           # r3 = address of c
    ld (r2,r1,4), r2    # r2 = a[(d+1) & 3]
    st r2, (r3,r0,4)    # c[d & 3] = r2 = a[(d+1) & 3]

    halt

.pos 0x200
# Data area

a:  .long 0             # a[0]
    .long 0             # a[1]
    .long 0             # a[2]
    .long 0             # a[3]
b:  .long 0             # b[0]
    .long 0             # b[1]
    .long 0             # b[2]
    .long 0             # b[3]
c:  .long 0             # c[0]
    .long 0             # c[1]
    .long 0             # c[2]
    .long 0             # c[3]
d:  .long 0             # d
