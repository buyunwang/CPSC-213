Calvin Cheng
36090132
o7x8

1.
0x69A2704D

2.
0x21B2503A

3.
0xEE7304659B8F58C0

4.
0x7098

5.
An example would be 0xABCDCDAB.
In general, numbers with the same big-endian and little-endian representation
must be a palindrome in terms of its byte representations. That is to say,
the first byte must be identical to the last byte, the second byte must be
identical to the penultimate byte, etc.

6.
The two processors are of different endianness. The first processor stores the
computation "1 + 65536" in memory (65537 in decimal) as 0x00100001, as it treats
the value in terms of its big endian representation.
The second processor, being a little endian system, accesses the memory at the
same address and reads the value as 0x1000100, since the bytes are processed in
reverse order. When it adds one to this value, then, the final result is
0x1000101, which happens to be 16777473 in decimal.


Machines tested (ISA - Endianness):
1) i386 - Little Endian
2) SPARC - Big Endian
3) x86_64 - Little Endian


For my implementation of MainMemory, all of my JUnit tests passed and provide
100% coverage.
