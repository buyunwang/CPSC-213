.pos 0x100
# Control flow instructions
# unconditional branch
br:       br 0x104                  # goto +1
br1:      br brfar                  # goto +127
br2:      br br1                    # goto -2
endbr:    halt                      # halt
# branch if equal
beq:      ld $0x5, r0               # r0 = 5
beq1:     beq r0, fail              # if r0==0, goto fail
beq2:     ld $0x0, r0               # r0 = 0
          beq r0, beq4              # if r0==0, goto +1
beq3:     beq r0, beqfar            # if r0==0, goto beqfar
beq4:     beq r0, beq3              # if r0==0, goto -2
endbeq:   halt                      # halt
# branch if greater
bgt:      ld $0xFFFFFFFF, r0        # r0 = -1
          bgt r0, fail              # if r0 > 0, goto fail
          inc r0                    # r0 = 0
          bgt r0, fail              # if r0 > 0, goto fail
          inc r0                    # r0 = 1
          bgt r0, bgt2              # if r0 > 0, goto +2
bgt1:     inc r0                    # r0 = 3
          bgt r0, bgtfar            # if r0 > 0, goto bgtfar
bgt2:     inc r0                    # r0 = 2
          bgt r0, bgt1              # if r0 > 0, goto -4
endbgt:   halt                      # halt
# jump
jmp:      j jmp2                    # goto jmp2
jmp1:     j 0x10000                 # goto 0x10000
jmp2:     j jmp1                    # goto jmp1

.pos 0x10000
jmpfar:   j endjmp                  # goto 0x654

.pos 0x654
endjmp:   halt                      # halt
# get pc
gpc:      gpc $0xE, r0              # r0 = pc + 0xE = 0x666
          gpc $0xC, r1              # r1 = pc + 0xC = 0x666
          gpc $0xA, r2              # r2 = pc + 0xA = 0x666
          gpc $0x8, r3              # r3 = pc + 0x8 = 0x666
          gpc $0x6, r4              # r4 = pc + 0x6 = 0x666
          gpc $0x4, r5              # r5 = pc + 0x4 = 0x666
          gpc $0x2, r6              # r6 = pc + 0x2 = 0x666
          gpc $0x0, r7              # r7 = pc = 0x666
          halt                      # halt
# indirect jump
ij:       ld $ijfar, r0             # r0 = &ijfar
          j 0(r0)                   # goto ijfar
ijback:   ld $0x300, r7             # r7 = 0x300
          j 0x00(r7)                # goto 0x300
back1:    j 0x0C(r7)                # goto 0x30C
back2:    j 0x18(r7)                # goto 0x318
back3:    halt                      # halt

.pos 0x150
# failure condition
fail:     halt                      # halt: test failed

.pos 0x202
# long branches
brfar:    br endbr                  # goto endbr
beqfar:   br endbeq                 # goto endbeq
bgtfar:   br endbgt                 # goto endbgt
ijfar:    j ijback                  # goto ijback

.pos 0x300
# indirect jump offsets
          ld $0x0, r0               # r0 = 0
          j back1                   # goto back1
          ld $0x1, r1               # r1 = 0
          j back2                   # goto back2
          ld $0x2, r2               # r2 = 0
          j back3                   # goto back3