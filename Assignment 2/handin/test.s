.pos 0x100
# Memory access
ldimm:    ld $0xA4ABE11E, r0        # r0 = 0xA4ABE11E
          ld $0xDEADC0DE, r1        # r1 = 0xDEADC0DE
          ld $a, r2                 # r2 = &a
          ld $b, r3                 # r3 = &b
ldbo:     ld 0x0(r2), r4            # r4 = a
          ld 0x8(r3), r5            # r5 = b[2]
ldind:    ld $1, r6                 # r6 = 1
          ld (r3, r6, 4), r7        # r7 = b[1]
stbo:     st r1, 0x0(r2)            # a = 0xDEADC0DE
          st r0, 0xC(r3)            # b[3] = 0xA4ABE11E
stind:    ld $4, r6                 # r6 = 4
          st r4, (r3, r6, 4)        # b[4] = a
# ALU instructions
rrmov:    mov r0, r7                # r7 = 0xA4ABE11E
          mov r1, r6                # r6 = 0xDEADC0DE
add:      ld $0x3, r0               # r0 = 3
          ld $0x6, r1               # r1 = 6
          add r1, r0                # r0 = 3 + 6 = 9
          ld $0xFFFFFFFF, r0        # r0 = -1
          ld $0x1, r1               # r1 = 1
          add r1, r0                # r0 = -1 + 1 = 0
and:      ld $0xAA, r0              # r0 = 0xAA (0b10101010)
          ld $0x55, r1              # r1 = 0x55 (0b01010101)
          and r1, r0                # r0 = 0xAA & 0x55 = 0
          ld $0xFF, r0              # r0 = 0xFF (0b11111111)
          and r1, r0                # r0 = 0xFF & 0x55 = 0x55
inc:      inc r4                    # r4 = 0x11223344 + 1
          ld $0xFFFFFFFE, r0        # r0 = -2
          inc r0                    # r0 = -2 + 1 = -1
          inc r0                    # r0 = -1 + 1 =  0
          inc r0                    # r0 =  0 + 1 =  1
dec:      dec r0                    # r0 =  1 - 1 =  0
          dec r0                    # r0 =  0 - 1 = -1
          dec r0                    # r0 = -1 - 1 = -2
          dec r4                    # r4 = 0x11223345 - 1
incad:    mov r3, r0                # r0 = &b[0]
          inca r0                   # r0 = &b[0] + 4 = &b[1]
          inca r0                   # r0 = &b[1] + 4 = &b[2]
decad:    deca r0                   # r0 = &b[2] - 4 = &b[1]
          deca r0                   # r0 = &b[1] - 4 = &b[0]
not:      ld $0xF0F00F0F, r0        # r0 = 0xF0F00F0F
          not r0                    # r0 = ~(0xF0F00F0F)
          ld $0x0, r0               # r0 = 0
          not r0                    # r0 = ~0 = 0xFFFFFFFF
shift:    ld $0x1, r0               # r0 = 1
          shl $0x1, r0              # r0 = 1  << 1 = 2
          shl $0x4, r0              # r0 = 2  << 4 = 32
          shr $0x2, r0              # r0 = 32 >> 2 = 8
          shr $0x3, r0              # r0 = 8  >> 3 = 1
nop:      nop
halt:     halt

.pos 0x1000
# Data area
a:        .long 0x11223344          # a
b:        .long 0xB000000B          # b[0]
          .long 0xB111111B          # b[1]
          .long 0xB222222B          # b[2]
          .long 0xB333333B          # b[3]
          .long 0xB444444B          # b[4]