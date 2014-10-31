.pos 0x100
start:           ld   $stackBtm, r5       # sp = address of last word of stack
                 inca r5                  # sp = address of word after stack
                 gpc  $6, r6              # ra = pc + 6
                 j    foo                 # call foo()
                 halt

.pos 0x200
foo:             deca r5                  # allocate calle part of foo's frame
                 st   r6, (r5)            # save ra on stack

                 ld   $1, r0              # r0 = 1, value of first arg
                 ld   $2, r1              # r1 = 2, value of second arg

                 gpc  $6, r6              # r6 = pc + 6
                 j    add                 # call add (1,2)

                 ld   $s, r1              # r1 = &s
                 st   r0, (r1)            # s = add (1,2)

                 ld   (r5), r6            # load ra from stack
                 inca r5                  # remove callee part of stack frame

                 j    (r6)                # return

.pos 0x300
add:             add  r1, r0              # return value = a + b
                 j    (r6)                # return

.pos 0x280
s:               .long 0x00000000         # s

.pos 0x1000
stackTop:        .long 0x00000000
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
                 .long 0x00000000         
stackBtm:        .long 0x00000000
