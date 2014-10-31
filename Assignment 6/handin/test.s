.pos 0x100
# double indirect jump - base + offset
dijbo1:   ld $table, r1             # r1 = address of jump table
          gpc $0x2, r6              # save return address
          j *0(r1)                  # call foo()
dijbo2:   gpc $0x2, r6              # save return address
          j *4(r1)                  # call bar()
dijbo3:   gpc $0x2, r6              # save return address
          j *16(r1)                  # call def()
          halt                      # halt
# double indrect jump - indexed
diji1:    ld $table, r1             # r1 = address of jump table
          ld $0, r2                 # r2 = 0
          gpc $0x2, r6              # save return address
          j *(r1, r2, 4)            # call foo()
diji2:    ld $2, r2                 # r2 = 2
          gpc $0x2, r6              # save return address
          j *(r1, r2, 4)            # call baz()
diji3:    ld $3, r2                 # r2 = 3
          gpc $0x2, r6              # save return address
          j *(r1, r2, 4)            # call abc()
diji4:    ld $4, r2                 # r2 = 4
          gpc $0x2, r6              # save return address
          j *(r1, r2, 4)            # call def()
          halt                      # halt

.pos 0x1000
# jump table
table:    .long foo
          .long bar
          .long baz
          .long abc
          .long def

# function definitions
.pos 0x2000
foo:      ld $0xF00, r0             # r0 = 0xF00
          j (r6)                    # return (0xF00 in r0)
.pos 0x4000
bar:      ld $0xBA13, r0            # r0 = 0xBA13
          j (r6)                    # return (0xBA13 in r0)
.pos 0x8000
baz:      ld $0xBA2, r0             # r0 = 0xBA2
          j (r6)                    # return (0xBA2 in r0)
.pos 0xABC00
abc:      ld $0xABC, r0             # r0 = 0xABC
          j (r6)                    # return (0xABC in r0)
.pos 0xDEF00
def:      ld $0xDEF, r0             # r0 = 0xDEF
          j (r6)                    # return (0xDEF in r0)