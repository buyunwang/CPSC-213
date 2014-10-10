Calvin Cheng
36090132
o7x8

Snippet S4 Observations
  First, the address of d0 is loaded into register 0, and the
ensuing instruction accesses d0.f by using the base+offset load
instruction, as the offset of d0.f is 4. Then, the value of
d0.f is stored into d0.e with the store base+offset
instruction, as the offset of d0.e is 0.
  Then, the address of d1 is loaded into register 0. Since d1
is a pointer to a dynamically allocated struct D, another load
instruction is used to dereference the address and access the
location of the struct. Once this is done, the instructions
are identical to the two above: the value of d1->f is retrieved
via the load base+offset instruction, and then stored into the
memory location at d1->e with the store base+offset
instruction.


Question 2a
One memory read:
- the value at the address s.x[i]

Question 2b
Two memory reads:
- the address specified by the value at the address of s.y
- the value at the ith index of the array pointed to by the
  value of s.y (i.e. s.y[i])

Question 2c
Two memory reads:
- the address specified by the value at the address of s.z
- the value of the ith index of the array x in the struct
  pointed to by s.z (i.e. s.z->x[i])