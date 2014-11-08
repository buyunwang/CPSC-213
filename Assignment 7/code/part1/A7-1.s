.address 0x0
main:            ld   $0x1028, r5         # r5 = address after sb
                 ld   $0xfffffff4, r0     # r0 = -12 = -(sizeof foo's frame)
                 add  r0, r5              # allocate foo's frame (caller)
                 ld   $0x200, r0          # r0 = &param1
                 ld   0x0(r0), r0         # r0 = param1
                 st   r0, 0x0(r5)         # a = param1
                 ld   $0x204, r0          # r0 = &param2
                 ld   0x0(r0), r0         # r0 = param2
                 st   r0, 0x4(r5)         # b = param2
                 ld   $0x208, r0          # r0 = &param3
                 ld   0x0(r0), r0         # r0 = param3
                 st   r0, 0x8(r5)         # c = param3
                 gpc  $6, r6              # save ra
                 j    0x300               # call foo(param1, param2, param3)
                 ld   $0x20c, r1          # r1 = &val
                 st   r0, 0x0(r1)         # val = foo(param1, param2, param3)
                 halt                     # end program
.address 0x200
param1:          .long 0x00000000         # param1
param2:          .long 0x00000000         # param2
param3:          .long 0x00000000         # param3
val:             .long 0x00000000         # val
.address 0x300
foo:             ld   0x0(r5), r0         # r0 = a
                 ld   0x4(r5), r1         # r1 = b
                 ld   0x8(r5), r2         # r2 = c
                 ld   $0xfffffff6, r3     # r3 = -10
                 add  r3, r0              # r0 = a - 10
                 mov  r0, r3              # r3 = a - 10
                 not  r3                  # r3 = ~(a - 10)
                 inc  r3                  # r3 = ~(a - 10) + 1 = -(a - 10)
                 bgt  r3, DEFAULT         # if (a < 10) goto DEFAULT
                 mov  r0, r3              # r3 = a - 10
                 ld   $0xfffffff8, r4     # r4 = -8
                 add  r4, r3              # r3 = a - 18
                 bgt  r3, DEFAULT         # if (a > 18) goto DEFAULT
                 ld   $0x400, r3          # r3 = &jmptable[0]
                 j    *(r3, r0, 4)        # goto jmptable[a - 10]
.address 0x330
case10:          add  r1, r2              # r2 = b + c
                 br   END_SWITCH          # goto END_SWITCH
case12:          not  r2                  # r2 = ~c
                 inc  r2                  # r2 = ~c + 1 = -c
                 add  r1, r2              # r2 = b - c
                 br   END_SWITCH          # goto END_SWITCH
case14:          not  r2                  # r2 = ~c
                 inc  r2                  # r2 = ~c + 1 = -c
                 add  r1, r2              # r2 = b - c
                 bgt  r2, L0              # if (b > c) goto L0
                 ld   $0x0, r2            # r2 = 0
                 br   L1                  # goto L1
L0:              ld   $0x1, r2            # r2 = 1
L1:              br   END_SWITCH          # goto END_SWITCH
case16:          not  r1                  # r1 = ~b
                 inc  r1                  # r1 = -b
                 add  r2, r1              # r1 = - b + c
                 bgt  r1, L2              # if (b < c) goto L2
                 ld   $0x0, r2            # r2 = 0
                 br   L3                  # goto L3
L2:              ld   $0x1, r2            # r2 = 1
L3:              br   END_SWITCH          # goto END_SWITCH
case18:          not  r2                  # r2 = ~c
                 inc  r2                  # r2 = -c
                 add  r1, r2              # r2 = b - c
                 beq  r2, L4              # if (b == c) goto L4
                 ld   $0x0, r2            # r2 = 0
                 br   L5                  # goto L5
L4:              ld   $0x1, r2            # r2 = 1
L5:              br   END_SWITCH          # goto END_SWITCH
DEFAULT:         ld   $0x0, r2            # r2 = 0
                 br   END_SWITCH          # goto END_SWITCH
END_SWITCH:      mov  r2, r0              # r0 = (computed in switch statement)
                 j    0x0(r6)             # return (value in r0 from switch)
.address 0x400
jmptable:        .long 0x00000330         # case a==10
                 .long 0x00000384         # case DEFAULT
                 .long 0x00000334         # case a==12
                 .long 0x00000384         # case DEFAULT
                 .long 0x0000033c         # case a==14
                 .long 0x00000384         # case DEFAULT
                 .long 0x00000354         # case a==16
                 .long 0x00000384         # case DEFAULT
                 .long 0x0000036c         # case a==18
.address 0x1000
                 .long 0x00000000         # stack
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
sb:              .long 0x00000000
