.pos 0x0
start:           ld   $sb, r5           # sp = address of stack bottom
                 inca r5                # sp = address after stack bottom
                 gpc  $6, r6            # ra = pc + 6
                 j    0x300             # call main()
                 halt                   # end program
.pos 0x100
                 .long 0x00001000       # ptr = &arr
.pos 0x200
add_at:          ld   0x0(r5), r0       # r0 = val
                 ld   0x4(r5), r1       # r1 = ind
                 ld   $0x100, r2        # r2 = 0x100 = &ptr
                 ld   0x0(r2), r2       # r2 = ptr = pointer to arr
                 ld   (r2, r1, 4), r3   # r3 = ptr[ind]
                 add  r3, r0            # r0 = val + ptr[ind]
                 st   r0, (r2, r1, 4)   # ptr[ind] = val + ptr[ind]
                 j    0x0(r6)           # return
.pos 0x300
main:            ld   $0xfffffff4, r0   # r0 = -12 = -(size of main's frame)
                 add  r0, r5            # allocate main's frame (callee)
                 st   r6, 0x8(r5)       # save ra on stack

                 ld   $0x1, r0          # r0 = 1
                 st   r0, 0x0(r5)       # a = 1
                 ld   $0x2, r0          # r0 = 2
                 st   r0, 0x4(r5)       # b = 2

                 ld   $0xfffffff8, r0   # r0 = -8 = -(sizeof add_at's frame)
                 add  r0, r5            # allocate add_at's frame (caller)

                 ld   $0x3, r0          # r0 = 3
                 st   r0, 0x0(r5)       # val = 3
                 ld   $0x4, r0          # r0 = 4
                 st   r0, 0x4(r5)       # ind = 4

                 gpc  $6, r6            # ra = pc + 6
                 j    0x200             # call add_at(3, 4)

                 ld   $0x8, r0          # r0 = 8 = sizeof add_at's frame
                 add  r0, r5            # deallocate add_at's frame (caller)

                 ld   0x0(r5), r1       # r1 = a
                 ld   0x4(r5), r2       # r2 = b

                 ld   $0xfffffff8, r0   # r0 = -8 = -(sizeof add_at's frame)
                 add  r0, r5            # allocate add_at's frame (caller)

                 st   r1, 0x0(r5)       # val = a
                 st   r2, 0x4(r5)       # ind = b

                 gpc  $6, r6            # ra = pc + 6
                 j    0x200             # call add_at(a, b)

                 ld   $0x8, r0          # r0 = 8 = sizeof add_at's frame
                 add  r0, r5            # deallocate add_at's frame (caller)

                 ld   0x8(r5), r6       # load return address from stack
                 ld   $0xc, r0          # r0 = 12 = sizeof main's frame
                 add  r0, r5            # deallocate main's frame (callee)
                 j    0x0(r6)           # return
.pos 0x1000
                 .long 0x00000000       # arr[0]
                 .long 0x00000000       # arr[1]
                 .long 0x00000000       # arr[2]
                 .long 0x00000000       # arr[3]
                 .long 0x00000000       # arr[4]
                 .long 0x00000000       # arr[5]
                 .long 0x00000000       # arr[6]
                 .long 0x00000000       # arr[7]
                 .long 0x00000000       # arr[8]
                 .long 0x00000000       # arr[9]
.pos 0x8000
# These are here so you can see (some of) the stack contents.
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
sb: .long 0
