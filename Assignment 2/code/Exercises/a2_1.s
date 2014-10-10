.pos 0x100
        # c = 5
        ld $c, r7                 # r7 = &c
        ld $0x5, r0               # r0 = 5
        st r0, 0x0(r7)            # c = 5
        # b = c + 10
        ld $c, r7                 # r7 = &c
        ld 0x0(r7), r0            # r0 = c
        ld $0xA, r1               # r1 = 10
        add r1, r0                # r0 = c + 10
        ld $b, r6                 # r6 = &b
        st  r0, 0x0(r6)           # b = c + 10
        # a[8] = 8
        ld $a, r7                 # r7 = &a
        ld $0x8, r0               # r0 = 8
        st r0, (r7, r0, 4)        # a[8] = 8
        # a[4] = a[4] + 4
        ld $a, r7                 # r7 = &a
        ld $0x4, r4               # r4 = 4
        ld (r7, r4, 4), r0        # r0 = a[4]
        add r4, r0                # r0 = a[4] + 4
        st r0, (r7, r4, 4)        # a[4] = a[4] + 4
        # a[c] = a[8] + b + a[b & 0x7]
        ld $a, r7                 # r7 = &a
        ld $b, r6                 # r6 = &b
        ld 0x0(r6), r1            # r1 = b
        ld $0x7, r0               # r0 = 0x7
        and r1, r0                # r0 = b & 0x7
        ld (r7, r0, 4), r2        # r2 = a[b & 0x7]
        ld 0x20(r7), r0           # r0 = a[8]
        add r1, r0                # r0 = a[8] + b
        add r2, r0                # r0 = a[8] + b + a[b & 0x7]
        ld $c, r5                 # r5 = &c
        ld 0x0(r5), r1            # r1 = c
        st r0, (r7, r1, 4)        # a[c] = a[8] + b + a[b & 0x7]
        # end of program
        halt                      # halt

.pos 0x1000
# Data area
b:      .long 0                   # b
c:      .long 0                   # c
a:      .long 0                   # a[0]
        .long 0                   # a[1]
        .long 0                   # a[2]
        .long 0                   # a[3]
        .long 0                   # a[4]
        .long 0                   # a[5]
        .long 0                   # a[6]
        .long 0                   # a[7]
        .long 0                   # a[8]
        .long 0                   # a[9]