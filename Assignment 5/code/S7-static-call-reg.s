.pos 0x100
foo:             gpc  $6, r6              # r6 = pc + 6
                 j    ping                # call ping()
                 halt                     # halt
.pos 0x500
ping:            j    (r6)                # return
