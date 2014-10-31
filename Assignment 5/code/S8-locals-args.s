.pos 0x100
start:           ld   $stackBtm, r5       # sp = address of last word of stack
                 inca r5                  # sp = address of word after stack
                 gpc  $0x6, r6            # r6 = pc + 6
                 j    foo                 # call foo()
                 halt

.pos 0x200
foo:             deca r5                  # allocate callee part of foo's frame
                 st   r6, 0x0(r5)         # save ra on stack

                 ld   $0xfffffff4, r0     # r0 = -12 = -(size of caller part of b's frame)
                 add  r0, r5              # allocate caller part of b's frame
                 ld   $0x0, r0            # r0 = 0 = value of a0
                 st   r0, 0x0(r5)         # save value of a0 to stack
                 ld   $0x1, r0            # r0 = 1 = value of a1
                 st   r0, 0x4(r5)         # store value of a1 to stack
                 ld   $0x2, r0            # r0 = 2 = value of a2
                 st   r0, 0x8(r5)         # store value of a2 to stack

                 gpc  $0x6, r6            # set return address
                 j    b                   # b (0, 1, 2)

                 ld   $0x12, r0            # r0 = 12 = size of caller part of b's frame
                 add  r0, r5              # deallocate caller part of b's frame

                 ld   0x0(r5), r6         # load return address from stack
                 inca r5                  # deallocate callee part of foo's frame
                 j    0x0(r6)             # return

.pos 0x400
b:               ld   $0xfffffff4, r0     # r0 = -12 = -(size of callee part of b's frame)
                 add  r0, r5              # allocate callee part of b's frame
                 st   r6, 0x8(r5)         # store return address to stack

                 ld   0xc(r5), r0         # r0 = a0
                 st   r0, 0x0(r5)         # l0 = a0

                 ld   0x10(r5), r0        # r0 = a1
                 st   r0, 0x4(r5)         # l1 = a1

                 gpc  $0x6, r6            # set return address
                 j    c                   # c()

                 ld   0x8(r5), r6         # load return address from stack
                 ld   $0xc, r0            # r0 = 12 = size of callee part of b's frame
                 add  r0, r5              # deallocate callee parts of b's frame
                 j    0x0(r6)             # return

.pos 0x600
c:               j    0x0(r6)             # return

.pos 0x1000
stackTop:        .long 0x0                
                 .long 0x0                
                 .long 0x0                
                 .long 0x0                
                 .long 0x0                
                 .long 0x0
                 .long 0x0
                 .long 0x0                
                 .long 0x0
stackBtm:        .long 0x0
