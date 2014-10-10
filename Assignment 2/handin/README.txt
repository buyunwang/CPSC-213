Calvin Cheng
36090132
o7x8

My test strategy was fairly simple; I simply went through the entire list of
instructions, and made sure that each was working as intended. In the cases
where it was appropriate to test multiple cases (e.g. testing a literal as well
as a labelled value), I made sure to test multiple cases. Throughout the tests,
I also tried to make use of as many different registers as possible, which
served to test the implementation of the registers/memory classes as well. I
stepped through the entire test file, and there did not seem to be any issues
or discrepancies with the expected results of each instruction, so I concluded
that my implementation of the CPU class is correct.

Snippet 1: "S1-global-static.s"
The data is first all initialized to -1; that is, the value of a and each int in
the array b is set to -1.
In the code, 0 is loaded into register r0. Then, the address of a is loaded into
register 1. The store instruction then puts the value in register r0 into the
memory at the address specified by r1 (the address of a). Thus, a = 0.
Next, the address of b and a are loaded into registers r0 and r1, respectively.
The value of a is then loaded into register r2, and this value is then used in
the following instruction to store a into the a'th position in the array b.
Thus, as represented in C code, b[a] = a.

Snippet 2: "S2-global-dyn-array.s"
Again, the data is all initialized to -1, with the exception of b, as in this
case, b is a pointer to a dynamically-allocated array, and thus holds the
address to said array.
In the code, a is assigned the value 0 via the same process above; the number 0
is loaded into r0, the address of a is loaded into r1, and the store instruction
stores the number 0 into memory at the address of a.
In the next line, then, the address of b is first loaded into r0. However,
because b is only a pointer holding the address of the actual dynamically-
allocated array, another load instruction is required to load the value b
into register r0. Only now does the register contain the address of the first
element of the dynamically-allocated array.
The next two instructions store the value of a into r2 using register r1 to
hold the address of a.
Finally, the value of a is stored into the dynamically-allocated array at the
index a in the final instruction (before the halt), using the store indexed
instruction.
