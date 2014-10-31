.pos 0x100
start:           ld   $stackBtm, r5       # sp = address of last word of stack
                 inca r5                  # sp = address of word after stack
                 gpc  $6, r6              # ra = pc + 6
                 j    foo                 # call foo()
                 halt

.pos 0x200
foo:             deca r5                  # allocate calle part of foo's frame
                 st   r6, (r5)            # save ra on stack

                 ld   $0xfffffff8, r0     # r0 = -8 = -(size of caller part of add's frame)
                 add  r0, r5              # allocate caller part of add's frame
                 ld   $1, r0              # r0 = 1, value of first arg
                 st   r0, 0(r5)           # save first arg on stack
                 ld   $2, r0              # r0 = 2, value of second arg
                 st   r0, 4(r5)           # save second arg on stack

                 gpc  $6, r6              # r6 = pc + 6
                 j    add                 # call add (1,2)

                 ld   $8, r1              # r0 = 8 = size of caller part of add's frame
                 add  r1, r5              # deallocate caller part of add's frame

                 ld   $s, r1              # r1 = &s
                 st   r0, (r1)            # s = add (1,2)

                 ld   (r5), r6            # load ra from stack
                 inca r5                  # remove callee part of stack frame

                 j    (r6)                # return

.pos 0x300
add:             ld   0(r5), r0           # r0 = arg0
                 ld   4(r5), r1           # r1 = arg1
                 add  r1, r0              # return (r0) = a (r0) + b (r1)
                 j    (r6)                # return

.pos 0x400
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
