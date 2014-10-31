.pos 0x100
start:
    ld $sb, r5          # sp = address of stack bottom
    inca    r5          # sp = address after stack bottom
    gpc $6, r6          # ra = pc + 6
    j main              # call main()
    halt                # end program

f:
    ld $0, r0           # r0 = count = 0
    ld (r5), r1         # r1 = n
    ld $0x80000000, r2  # r2 = 0x80000000
f_loop:
    beq r1, f_end       # if (n == 0) goto f_end
    mov r1, r3          # r3 = n
    and r2, r3          # r3 = n & 0x80000000
    beq r3, f_if1       # if (n & 0x80000000 == 0) goto f_if1
    inc r0              # count++
f_if1:
    shl $1, r1          # n <<= 1
    br f_loop           # goto f_loop
f_end:
    j(r6)               # return (count in r0)

main:
    deca r5             # decrement sp
    st r6, (r5)         # save ra on stack
    deca r5             # decrement sp
    st r4, (r5)         # save r4 on stack
    ld $8, r4           # r4 = i = 8
    deca r5             # decrement sp
main_loop:
    beq r4, main_end    # if (i == 0) goto main_end
    dec r4              # i -= 1
    ld $x, r0           # r0 = &x
    ld (r0,r4,4), r0    # r0 = x[i]
    st r0, (r5)         # n = x[i]
    gpc $6, r6          # ra = pc + 6
    j f                 # call f(x[i]) - return in r0
    ld $y, r1           # r1 = &y
    st r0, (r1,r4,4)    # y[i] = f(x[i])
    br main_loop        # goto main_loop
main_end:
    inca r5             # increment sp
    ld (r5), r4         # load r4 from stack
    inca r5             # increment sp
    ld (r5), r6         # load ra from stack
    inca r5             # increment sp
    j (r6)              # return

.pos 0x2000
x:
    .long 1             # x[0]
    .long 2             # x[1]
    .long 3             # x[2]
    .long 0xffffffff    # x[3]
    .long 0xfffffffe    # x[4]
    .long 0             # x[5]
    .long 184           # x[6]
    .long 340057058     # x[7]

y:
    .long 0             # y[0]
    .long 0             # y[1]
    .long 0             # y[2]
    .long 0             # y[3]
    .long 0             # y[4]
    .long 0             # y[5]
    .long 0             # y[6]
    .long 0             # y[7]

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
