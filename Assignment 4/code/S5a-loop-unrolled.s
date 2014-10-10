.pos 0x100
                 ld   $0x0, r0            # r0 = 0 (temp for i)
                 ld   $a, r1              # r1 = address of a[0]
                 ld   $0x0, r2            # r2 = 0 (temp for s)
                 ld   (r1, r0, 4), r3     # r3 = a[0]
                 add  r3, r2              # r2 += a[0]
                 inc  r0                  # r0++
                 ld   (r1, r0, 4), r3     # r3 = a[1]
                 add  r3, r2              # r2 += a[1]
                 inc  r0                  # r0++
                 ld   (r1, r0, 4), r3     # r3 = a[2]
                 add  r3, r2              # r2 += a[2]
                 inc  r0                  # r0++
                 ld   (r1, r0, 4), r3     # r3 = a[3]
                 add  r3, r2              # r2 += a[3]
                 inc  r0                  # r0++
                 ld   (r1, r0, 4), r3     # r3 = a[4]
                 add  r3, r2              # r2 += a[4]
                 inc  r0                  # r0++
                 ld   (r1, r0, 4), r3     # r3 = a[5]
                 add  r3, r2              # r2 += a[5]
                 inc  r0                  # r0++
                 ld   (r1, r0, 4), r3     # r3 = a[6]
                 add  r3, r2              # r2 += a[6]
                 inc  r0                  # r0++
                 ld   (r1, r0, 4), r3     # r3 = a[7]
                 add  r3, r2              # r2 += a[7]
                 inc  r0                  # r0++
                 ld   (r1, r0, 4), r3     # r3 = a[8]
                 add  r3, r2              # r2 += a[8]
                 inc  r0                  # r0++
                 ld   (r1, r0, 4), r3     # r3 = a[9]
                 add  r3, r2              # r2 += a[9]
                 inc  r0                  # r0++
                 ld   $s, r0              # r0 = address of s
                 st   r2, 0x0(r0)         # s = r2 (temp for s)
                 halt                     
.pos 0x1000
s:               .long 0x00000000         # s
.pos 0x1008
a:               .long 0x00000002         # a[0]
                 .long 0x00000004         # a[1]
                 .long 0x00000006         # a[2]
                 .long 0x00000008         # a[3]
                 .long 0x0000000a         # a[4]
                 .long 0x0000000c         # a[5]
                 .long 0x0000000e         # a[6]
                 .long 0x00000010         # a[7]
                 .long 0x00000012         # a[8]
                 .long 0x00000014         # a[9]
