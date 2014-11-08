.pos 0x100
foo:      ld   $i, r0           # r0 = &i
          ld   0x0(r0), r0      # r0 = i
          ld   $0xffffffed, r1  # r1 = -19
          add  r0, r1           # r0 = i-19
          bgt  r1, l0           # goto l0 if i>19
          br   default          # goto default if i<20
l0:       ld   $0xffffffe9, r1  # r1 = -23
          add  r0, r1           # r1 = i-23
          bgt  r1, default      # goto default if i>23
          ld   $0xffffffec, r1  # r1 = -20
          add  r1, r0           # r0 = i-20
          ld   $jmptable, r1    # r1 = &jmptable
          j    *(r1, r0, 4)     # goto jmptable[i-20]
cont:     halt                  # first statment following switch
.pos 0x140
case20:   ld   $0xa, r1         # r1 = 10
          br   done             # goto done
case21:   ld   $0xb, r1         # r1 = 11
          br   done             # goto done
case23:   ld   $0xd, r1         # r1 = 13
          br   done             # goto done
default:  ld   $0xe, r1         # r1 = 14
          br   done             # goto done
done:     ld   $j, r0           # r0 = &j
          st   r1, 0x0(r0)      # j = r1
          j   cont              # goto cont
.pos 0x800
jmptable: .long case20          # code address for case 20
          .long case21          # code address for case 21
          .long default         # code address for case 22
          .long case23          # code address for case 23
.pos 0x1000
i:        .long 0x00000016      # i = 0x16 = 22
j:        .long 0x00000000      # j
