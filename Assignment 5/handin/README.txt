Calvin Cheng
36090132
o7x8

1a
The major difference between "S7-static-call-reg.s" and "S7-static-call-stack.s"
is that in the latter program, the function ping() stores the return address of
the function onto the stack. Of course, in the former program, no memory is ever
even allocated for a stack; regardless, the use of the stack pointer to store
return addresses onto the stack is the main difference between the two programs.

1b
The advantages of the former program (whose ping() function does not store the
return address into memory on the stack) is that it is much faster; as memory
reads/writes are fairly time-consuming, eliminating such a step would speed the
program up considerably.
However, this is not without drawbacks; the main advantage of the latter program
is that it allows further function calls. By storing the return address in r[6]
onto the stack, ping() will then be able to call other functions. If the return
address of the function were not saved prior to a another function call, the
address would be over-written, and ping() would not know to where it should
return.

2a
Lines 13 and 31

2b
Lines 23 and 45

3a
In "s9-args-stack.s", the two arguments of foo() are passed in the stack,
whereas in "s9-args-regs", the arguments are passed in registers r[0] and r[1].
In particular, in "s9-args-stack.s", two store operations are made in the
caller prologue--one for each parameter passed to the function. Then, in the
add() function, two memory reads are used to load the two arguments that were
passed into the function. In "s9-args-regs.s", this is not necessary, as the
function simply uses the values that were already in r[0] and r[1].

3b
Assuming storing to memory is considered a memory "access", "s9-args-stack.s"
requires access to the memory four more times than "s9-args-regs.s"; there is an
extra read/write step to the memory for each argument in the function, and as
add() takes two parameters, there are two extra memory reads and two extra
memory writes in "s9-args-stack.s".

5
The virus works by exploiting the lack of bounds checking for the dst array. As
the local array dst in copy() is initialized only to have two elements, by
passing in an array with more than two elements, one will be able to overwrite
ensuing values stored in the stack.
In this case, the datum following the dst array is the return address. As a
result, one will be able to overwrite the current return address of count(),
given that the size of src is greater than two. If src[2] contains the address
of the first instruction of the exploit, then the program will jump to the
specified line after the termination of count(). In this case, src[2]
contains the address in the stack that would correspond to src[3] onwards,
after each element in src has been copied into the stack. The final element in
src is 0, as the null-termination is a requirement for the while loop in copy()
to terminate.
Starting in src[3] is the executable code to run the exploit. The value of -1 is
first loaded into r0, and then copied into each of the registers sequentially.
These instructions are encoded in machine language as various .long directives,
finally ending with a halt instruction.
Given this code, once the count() function reaches its final instruction, it
jumps to the newly-replaced address containing the exploit, which promptly
sets all of the registers to -1 and halts.
