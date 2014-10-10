.pos 0x100
                 ld   $0x0, r0            # r0 = temp_i = 0
                 ld   $a, r1              # r1 = address of a[0]
                 ld   $0x0, r2            # r2 = temp_s = 0
                 ld   $0xfffffff6, r4     # r4 = -10
loop:            mov  r0, r5              # r5 = temp_i
                 add  r4, r5              # r5 = temp_i-10
                 beq  r5, end_loop        # if temp_i=10 goto +4
                 ld   (r1, r0, 4), r3     # r3 = a[temp_i]
                 add  r3, r2              # temp_s += a[temp_i]
                 inc  r0                  # temp_i++
                 br   loop                # goto -7
end_loop:        ld   $s, r1              # r1 = address of s
                 st   r2, 0x0(r1)         # s = temp_s
                 st   r0, 0x4(r1)         # i = temp_i
                 halt                     
.pos 0x1000
s:               .long 0x00000000         # s
i:               .long 0x00000000         # i
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
