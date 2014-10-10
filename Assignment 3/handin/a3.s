.pos 0x1000
code:
        # v = s.x[i]
        ld $i, r0             # r0 = &i
        ld (r0), r0           # r0 = i
        ld $s, r1             # r1 = &s
        ld (r1, r0, 4), r1    # r1 = s.x[i]
        ld $v, r0             # r0 = &v
        st r1, (r0)           # v = s.x[i]
        # v = s.y[i]
        ld $i, r0             # r0 = &i
        ld (r0), r0           # r0 = i
        ld $s, r1             # r1 = &s
        ld 8(r1), r1          # r1 = s.y
        ld (r1, r0, 4), r1    # r1 = s.y[i]
        ld $v, r0             # r0 = &v
        st r1, (r0)           # v = s.y[i]
        # v = s.z->x[i]
        ld $i, r0             # r0 = &i
        ld (r0), r0           # r0 = i
        ld $s, r1             # r1 = &s
        ld 12(r1), r1         # r1 = s.z
        ld (r1, r0, 4), r1    # r1 = s.z->x[i]
        ld $v, r0             # r0 = &v
        st r1, (r0)           # v = s.z->x[i]
        # finish program
        halt                  # halt

.pos 0x2000
static:
i:      .long 0       # i
v:      .long 0       # j
s:      .long 0       # s.x[0]
        .long 0       # s.x[1]
        .long heap0   # s.y
        .long heap1   # s.z

.pos 0x3000
heap0:  .long 0       # s.y[0]
        .long 0       # s.y[1]
        .long 0       # s.y[2]
heap1:  .long 0       # s.z->x[0]
        .long 0       # s.z->x[1]
        .long 0       # s.z->y
        .long 0       # s.z->z