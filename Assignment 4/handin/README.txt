Calvin Cheng
36090132
o7x8

Test 2:
Cause: ip was freed before the while guard attempted to access it
Fix: moved free(ip) to after while loop

Test 4:
Cause: ip was freed before the while guard attempted to access it
Fix: moved free(ip) to after while loop

Test 6:
Cause: In the pop() function, e was freed before returning e->name
Fix: The pop() function was modified to take a pointer to a char array as a
     parameter, in which e->name is stored. This was to avoid the issue of
     freeing before returning. All of the tests were then modified to use a
     local variable whose address was passed into the function. This fixed
     all of the memory allocation errors, as verified by valgrind.


Implementation of test.s
All of my tests passed, so I believe that my implementation is correct.


Snippet S5 & S5a - Loops
In Snippet S5a, the loop has been "unrolled" so that every iteration of the
loop has been explicitly written into the assembly file. The value of r0 and
r2 are initialized to 0, and for the number of iterations specified (in this
case, 10), the variable s is incremented by the i-th element in the array.
Finally, the value of s is stored back into memory.
In Snippet S5, the loop remains in the assembly code. Similar to above, the
variable s is incremented by the i-th element of the array. One thing to note,
however, is that in this case, there is a branch instruction. Prior to each
execution of the loop, the instruction checks if i - 10 == 0. If so, the
program counter jumps to the end, where the values are written back into
memory. At the end of the loop, there is an unconditional branch statement
to bring the program back to the top of the loop, to be evaluated by the loop
guard once again.

Snippet S6 - If
In this snippet, a very simple if statement is illustrated. The values of a and
b are first loaded into registers r0 and r1. Then, their difference is taken
and the result is passed as an operand to the instruction bgt. If the result
is greater than 0, then the conditional branch causes the program counter to
skip past the else segment and execute the following instructions, writing a
into memory as the max value. If the result is less than 0, however, b is
written into the temp_max variable, which is then stored into memory.

Snippet S7 - static-call-reg
This snippet illustrates the basics of a function call. First, an address that
refers to the return address is stored in r6. The program flow then jumps to
ping, at which point the program counter is instructed to point to the address
at r6, which was the intended location for the function to return. The program
finishes at that point.