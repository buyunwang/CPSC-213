.pos 0x0
max:             .long 0xffffffff
i:               .long 0x0
a:               .long 0x2                
                 .long 0x28               
                 .long 0x6                
                 .long 0x50               
                 .long 0xa                
                 .long 0x78               
                 .long 0xe                
                 .long 0x10               
                 .long 0x12               
                 .long 0x14
.pos 0x50
start:           ld   $0x0, r0            # r0 = t_i = 0
                 ld   $0xfffffff6, r1     # r1 = -10
                 ld   $max, r2            # r2 = &max
                 ld   0x0(r2), r3         # r3 = max = t_max
                 ld   $a, r4              # r4 = &a[0]
loop:            mov  r0, r5              # r5 = t_i
                 add  r1, r5              # r5 = t_i-10
                 beq  r5, done            # goto done if t_i==10
                 ld   (r4, r0, 4), r5     # r5 = a[t_i]
                 mov  r5, r7              # r7 = a[t_i]
                 not  r7
                 inc  r7                  # r7 = -a[t_i]
                 add  r3, r7              # r7 = t_max - a[t_i]
                 bgt  r7, cont            # goto cont if a[t_i] < t_max
                 mov  r5, r3              # max = a[i] if a[t_i] >= t_max
cont:            inc  r0                  # t_i = t_i + 1
                 br   loop                # goto loop
done:            st   r3, 0x0(r2)         # max = t_max
                 ld   $i, r1              # r1 = &i
                 st   r0, 0x0(r1)         # i = t_i
                 halt
