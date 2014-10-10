.pos 0x100
                 ld   $a, r0              # r0 = &a
                 ld   0x0(r0), r0         # r0 = a
                 ld   $b, r1              # r1 = &b
                 ld   0x0(r1), r1         # r1 = b
                 mov  r1, r2              # r2 = b
                 not  r2                  # temp_c = ! b
                 inc  r2                  # temp_c = - b
                 add  r0, r2              # temp_c = a-b
                 bgt  r2, then            # if (a>b) goto +2
else:            mov  r1, r3              # temp_max = b
                 br   end_if              # goto +1
then:            mov  r0, r3              # temp_max = a
end_if:          ld   $max, r0            # r0 = &max
                 st   r3, 0x0(r0)         # max = temp_max
                 halt                     
.pos 0x1000
a:               .long 0x00000001         # a
.pos 0x2000
b:               .long 0x00000002         # b
.pos 0x3000
max:             .long 0x00000000         # max
