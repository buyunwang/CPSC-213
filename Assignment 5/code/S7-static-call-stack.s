.pos 0x100
foo:             ld   $0x1028, r5         # initialize stack pointer
                 gpc  $6, r6              # ra = pc + 6
                 j    ping                # call ping()
                 halt                     # halt
.pos 0x500
ping:            deca r5                  # sp -= 4
                 st   r6, (r5)            # *sp = ra
                 ld   (r5), r6            # ra = *sp
                 inca r5                  # sp += 4
                 j (r6)                   # return
.pos 0x1000
                .long 0x00000000         
                .long 0x00000000         
                .long 0x00000000         
                .long 0x00000000         
                .long 0x00000000         
                .long 0x00000000         
                .long 0x00000000         
                .long 0x00000000         
                .long 0x00000000         
                .long 0x00000000         


