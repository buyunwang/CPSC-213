.pos 0x100
start:      ld    $sb, r5         # r5 = address of last word of stack
            inca  r5              # r5 = address of first word after stack
            gpc   $0x6, r6        # ra = pc + 6
            j     main            # call main
            halt                  # end program

.pos 0x200
main:       deca  r5              # allocate main's frame (callee)
            st    r6, (r5)        # save ra on stack

            ld    $0xFFFFFFF4, r0 # r0 = -12 = -(sizeof compute's frame)
            add   r0, r5          # allocate compute's frame (caller)
            ld    $cmds, r0       # r0 = address of cmds
            st    r0, (r5)        # c = address of cmds
            ld    $soln, r0       # r0 = address of soln
            st    r0, 4(r5)       # s = address of soln
            ld    $4, r0          # r0 = 4 = length of cmds[]
            st    r0, 8(r5)       # n = 4
            gpc   $6, r6          # set ra
            j     compute         # call compute(cmds, soln, length of cmds[])

            ld    $12, r0         # r0 = 12 = sizeof compute's frame
            add   r0, r5          # deallocate compute's frame (caller)

            ld    (r5), r6        # load ra from stack
            inca  r5              # deallocate main's frame (callee)
            j     (r6)            # return

.pos 0x300
compute:    deca  r5              # allocate compute's frame (callee)
            st    r6, (r5)        # save ra on stack

            ld    12(r5), r2      # r2 = i = n

for_loop:   beq   r2, loop_end    # if (i == 0) goto loop_end
            dec   r2              # i--

            ld    4(r5), r1       # r1 = &c[0]
            mov   r2, r0          # r0 = i
            shl   $3, r0          # r0 = i << 3 = 8 * i
            mov   r2, r3          # r3 = i
            shl   $2, r3          # r3 = i << 2 = 4 * i
            add   r3, r0          # r0 = 8*i + 4*i = 12*i
            add   r0, r1          # r1 = &c[0] + 12*i = &c[i]

            deca  r5              # allocate space for caller-save r2
            st    r2, (r5)        # save i (in r2) on stack

            ld    $0xFFFFFFF8, r0 # r0 = -8 = -(sizeof op's frame)
            add   r0, r5          # allocate op's frame (caller)
            ld    4(r1), r0       # r0 = c[i].a
            st    r0, 0(r5)       # a = c[i].a
            ld    8(r1), r0       # r0 = c[i].b
            st    r0, 4(r5)       # b = c[i].b
            gpc   $2, r6          # set ra
            j     *(r1)           # call c[i].op(c[i].a, c[i].b)

            ld    $8, r3          # r3 = 8 = (sizeof op's frame)
            add   r3, r5          # deallocate op's frame (caller)

            ld    (r5), r2        # load i from stack back onto r2
            inca  r5              # deallocate space for caller-save r2

            ld    8(r5), r1       # r1 = &s[0]
            st    r0, (r1, r2, 4) # s[i] = c[i].op(c[i].a, c[i].b)
            br    for_loop        # goto for_loop
            
loop_end:   ld    (r5), r6        # load ra from stack
            inca  r5              # deallocate compute's frame (callee)
            j     (r6)            # return

.pos 0x400
add:        ld    (r5), r0        # r0 = a
            ld    4(r5), r1       # r1 = b
            add   r1, r0          # r0 = a + b
            j     (r6)            # return a + b

.pos 0x500
sub:        ld    (r5), r0        # r0 = a
            ld    4(r5), r1       # r1 = b
            not   r1              # r1 = ~b
            inc   r1              # r1 = ~b + 1 = -b
            add   r1, r0          # r0 = a - b
            j     (r6)            # return a - b

.pos 0x600
inc:        ld    (r5), r0        # r0 = a
            inc   r0              # r0 = a + 1
            j     (r6)            # return a + 1

.pos 0x700
dec:        ld    (r5), r0        # r0 = a
            dec   r0              # r0 = a - 1
            j     (r6)            # return a - 1

.pos 0x800
cmds:       .long add             # cmds[0].op
            .long 5               # cmds[0].a
            .long 2               # cmds[0].b
            .long sub             # cmds[1].op
            .long 5               # cmds[1].a
            .long 2               # cmds[1].b
            .long inc             # cmds[2].op
            .long 5               # cmds[2].a
            .long 0               # cmds[2].b
            .long dec             # cmds[3].op
            .long 5               # cmds[3].a
            .long 0               # cmds[3].b
soln:       .long 0               # soln[0]
            .long 0               # soln[1]
            .long 0               # soln[2]
            .long 0               # soln[3]

.pos 0x900
            .long 0x0             # the stack
            .long 0x0
            .long 0x0
            .long 0x0
            .long 0x0
            .long 0x0
            .long 0x0
sb:         .long 0x0
