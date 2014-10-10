.pos 0x100
        # a[i] = a[i+1] + b[i+2]
        ld $a, r7             # r7 = &a
        ld $b, r6             # r6 = &b
        ld $i, r5             # r5 = &i
        ld 0x0(r5), r0        # r0 = i
        mov r0, r1            # r1 = i
        inc r1                # r1 = i + 1
        mov r1, r2            # r2 = i + 1
        inc r2                # r2 = i + 2
        ld (r7, r1, 4), r1    # r1 = a[i+1]
        ld (r6, r2, 4), r2    # r2 = b[i+2]
        add r2, r1            # r1 = a[i+1] + b[i+2]
        st r1, (r7, r0, 4)    # a[i] = a[i+1] + b[i+2]
        # d[i] = a[i] + b[i]
        ld $i, r7             # r7 = &i
        ld 0x0(r7), r7        # r7 = i
        ld $a, r0             # r0 = &a
        ld (r0, r7, 4), r0    # r0 = a[i]
        ld $b, r1             # r1 = &b
        ld (r1, r7, 4), r1    # r1 = b[i]
        add r1, r0            # r0 = a[i] + b[i]
        ld $d, r1             # r1 = &d
        ld 0x0(r1), r1        # r1 = d = &c
        st r0, (r1, r7, 4)    # d[i] = a[i] + b[i]
        #d[i] = a[b[i]] + b[a[i]]
        ld $i, r7             # r7 = &i
        ld 0x0(r7), r7        # r7 = i
        ld $a, r5             # r5 = &a
        ld $b, r6             # r6 = &b
        ld (r5, r7, 4), r3    # r3 = a[i]
        ld (r6, r7, 4), r4    # r4 = b[i]
        ld (r5, r4, 4), r0    # r0 = a[b[i]]
        ld (r6, r3, 4), r1    # r1 = b[a[i]]
        add r0, r1            # r1 = a[b[i]] + b[a[i]]
        ld $d, r0             # r0 = &d
        ld 0x0(r0), r0        # r0 = d = &c
        st r1, (r0, r7, 4)    # d[i] = a[b[i]] + b[a[i]]
        # d[b[i]] = b[a[i & 3] & 3] - a[b[i & 3] & 3] + d[i]
        ld $0x3, r3           # r3 = 3
        ld $d, r4             # r4 = &d
        ld 0x0(r4), r4        # r4 = d = &c
        ld $a, r5             # r5 = &a
        ld $b, r6             # r6 = &b
        ld $i, r7             # r7 = &i
        ld 0x0(r7), r7        # r7 = i
        mov r3, r0            # r0 = 3
        and r7, r0            # r0 = i & 3
        ld (r5, r0, 4), r0    # r0 = a[i & 3]
        and r3, r0            # r0 = a[i & 3] & 3
        ld (r6, r0, 4), r0    # r0 = b[a[i & 3]] & 3; name opA
        mov r3, r1            # r1 = 3
        and r7, r1            # r1 = i & 3
        ld (r6, r1, 4), r1    # r1 = b[i & 3]
        and r3, r1            # r1 = b[i & 3] & 3
        ld (r5, r1, 4), r1    # r1 = a[b[i & 3] & 3]
        not r1                # r1 = ~a[b[i & 3] & 3]
        inc r1                # r1 = -a[b[i & 3] & 3]; name opB
        add r1, r0            # r0 = opA + opB
        ld (r4, r7, 4), r1    # r1 = d[i]
        add r1, r0            # r0 = opA + opB + d[i]
        ld (r6, r7, 4), r1    # r1 = b[i]
        st r0, (r4, r1, 4)    # d[b[i]] = opA + opB + d[i]
        # finish program
        halt                  # halt

.pos 0x1000
# Data area
a:      .long 0               # a[0]
        .long 0               # a[1]
        .long 0               # a[2]
        .long 0               # a[3]
        .long 0               # a[4]
        .long 0               # a[5]
        .long 0               # a[6]
        .long 0               # a[7]
b:      .long 0               # b[0]
        .long 0               # b[1]
        .long 0               # b[2]
        .long 0               # b[3]
        .long 0               # b[4]
        .long 0               # b[5]
        .long 0               # b[6]
        .long 0               # b[7]
c:      .long 0               # c[0]
        .long 0               # c[1]
        .long 0               # c[2]
        .long 0               # c[3]
        .long 0               # c[4]
        .long 0               # c[5]
        .long 0               # c[6]
        .long 0               # c[7]
i:      .long 0               # i
d:      .long c               # d