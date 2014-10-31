.pos 0x100
main:            ld   $stackBtm, r5       # sp = address of last word of stack
                 inca r5                  # sp = address of word after stack

                 gpc  $0x6, r6            # set return address
                 j    copy                # call copy()

                 halt                     # end program

.pos 0x150
copy:            ld   $0xfffffff4, r0     # r0 = -12 = -(sizeof callee frame)
                 add  r0, r5              # allocate copy's frame (callee)
                 st   r6, 0x8(r5)         # save return address on stack

                 ld   $0, r2              # r2 = i = 0

loop:            ld   $src, r0            # r0 = &src
                 ld   (r0, r2, 4), r0     # r0 = src[i]
                 beq  r0, end_loop        # if src[i]==0 goto end_loop

                 st   r0, (r5, r2, 4)     # dst[i] = src[i]
                 inc  r2                  # i++

                 br   loop                # goto beginning of while loop

end_loop:        ld   $0, r0              # r0 = 0
                 st   r0, (r5, r2, 4)     # dst[i] = 0

                 ld   8(r5), r6           # load return address from stack
                 ld   $12, r0             # r0 = 12 = sizeof callee frame
                 add  r0, r5              # deallocate copy's frame (callee)
                 j    0x0(r6)             # return

.pos 0x200
src:             .long 0x1                # src[0]
                 .long 0x2                # src[1]
                 .long 0x25C              # src[2]
                 .long 0x0000FFFF         # src[3]
                 .long 0xFFFF6001         # src[4]
                 .long 0x60026003         # src[5]
                 .long 0x60046005         # src[6]
                 .long 0x60066007         # src[7]
                 .long 0xF0000000         # src[8]
                 .long 0                  # src[9]

.pos 0x250
stackTop:        .long 0x0
                 .long 0x0
stackBtm:        .long 0x0
