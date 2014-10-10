.pos 0x100
                 ld   $0x0, r0            # r0 = 0
                 ld   $a, r1              # r1 = address of a
                 st   r0, 0x0(r1)         # a = 0
                 ld   $b, r0              # r0 = address of b (which stores address of b[0])
                 ld   0x0(r0), r0         # r0 = pointer to array
                 ld   $a, r1              # r1 = address of a
                 ld   0x0(r1), r2         # r2 = a
                 st   r2, (r0, r2, 4)     # b[a] = a
                 halt                     # halt
.pos 0x1000
a:               .long 0xffffffff         # a
.pos 0x2000
b:               .long 0x00003000         # address of b[0]; loaded dynamically
.pos 0x3000
b_data:          .long 0xffffffff         # b[0]
                 .long 0xffffffff         # b[1]
                 .long 0xffffffff         # b[2]
                 .long 0xffffffff         # b[3]
                 .long 0xffffffff         # b[4]
                 .long 0xffffffff         # b[5]
                 .long 0xffffffff         # b[6]
                 .long 0xffffffff         # b[7]
                 .long 0xffffffff         # b[8]
                 .long 0xffffffff         # b[9]
