.pos 0x100
start:           ld   $sb, r5             # r5 = address of last word of stack
                 inca r5                  # r5 = address of first word after stack
                 gpc  $0x6, r6            # ra = pc + 6
                 j    bar                 # call bar
                 halt                     
.pos 0x200
bar:             deca r5                  # allocate space for frame
                 deca r5                  
                 st   r6, 0x4(r5)         # save return address on stack
foo_a:           gpc  $0x6, r6            
                 j    new_A               # call new_A()
                 deca r5                  
                 st   r0, 0x0(r5)         
                 gpc  $0x6, r6            
                 j    foo                 # foo (new_A())
                 inca r5                  
foo_b:           gpc  $0x6, r6            
                 j    new_B               # call new_B()
                 deca r5                  
                 st   r0, 0x0(r5)         
                 gpc  $0x6, r6            
                 j    foo                 # foo (new_B())
                 inca r5                  
foo_c:           deca r5                  
                 ld   $0x64, r0           
                 st   r0, 0x0(r5)         
                 gpc  $0x6, r6            
                 j    new_C               # call new_C(100)
                 inca r5                  
                 deca r5                  
                 st   r0, 0x0(r5)         
                 gpc  $0x6, r6            
                 j    foo                 # foo (new_C(100))
                 inca r5                  
done:            ld   0x4(r5), r6         # restore return address from stack
                 inca r5                  # discard callee part of frame
                 inca r5                  
                 j    0x0(r6)             # return
.pos 0x300
foo:             deca r5                  # allocate space for frame
                 st   r6, 0x0(r5)         # save return address on stack
                 ld   0x4(r5), r0         # r0 = a
                 ld   0x0(r0), r1         # r1 = a->class
                 deca r5
                 st   r0, (r5)
                 gpc  $0x2, r6            # ra = pc + 2
                 j    *0x4(r1)            # a->class->ping (this)
                 inca r5
                 ld   0x4(r5), r0         # r0 = a
                 ld   0x0(r0), r1         # r1 = a->class
                 deca r5
                 st   r0, (r5)
                 gpc  $0x2, r6            # ra = pc + 2
                 j    *0x8(r1)            # a->class->pong (this)
                 inca r5
                 ld   0x0(r5), r6         # restore return address from stack
                 inca r5                  # discard callee part of frame
                 j    0x0(r6)             # return
.pos 0x400
new_A:           deca r5                  # allocate space for frame
                 st   r6, 0x0(r5)         # save return address on stack
                 ld   $0x4, r0            # r0 = sizeof (struct A_object)
                 gpc  $0x6, r6            
                 j    malloc              # call malloc (sizeof (struct A_object))
                 ld   $A_class, r1        # r1 = &a_class
                 st   r1, 0x0(r0)         # a->class = a_class
                 ld   0x0(r5), r6         # restore return address from stack
                 inca r5                  # discard callee part of frame
                 j    0x0(r6)             # return
.pos 0x500
new_B:           deca r5                  # allocate space for frame
                 st   r6, 0x0(r5)         # save return address on stack
                 ld   $0x4, r0            # r0 = sizeof (struct B_object)
                 gpc  $0x6, r6            
                 j    malloc              # call malloc (sizeof (struct B_object))
                 ld   $B_class, r1        # r1 = &a_class
                 st   r1, 0x0(r0)         # b->class = b_class
                 ld   0x0(r5), r6         # restore return address from stack
                 inca r5                  # discard callee part of frame
                 j    0x0(r6)             # return
.pos 0x600
new_C:           deca r5                  # allocate space for frame
                 st   r6, 0x0(r5)         # save return address on stack
                 ld   $0x8, r0            # r0 = sizeof (struct C_object)
                 gpc  $0x6, r6            
                 j    malloc              # call malloc (sizeof (struct C_object))
                 ld   $C_class, r1        # r1 = &a_class
                 st   r1, 0x0(r0)         # c->class = c_class
                 ld   0x4(r5), r1         # r1 = c->id
                 st   r1, 0x4(r0)         # c->id = id
                 ld   0x0(r5), r6         # restore return address from stack
                 inca r5                  # discard callee part of frame
                 j    0x0(r6)             # return
.pos 0x700
malloc:          mov  r0, r2              # r2 = arg0, # bytes to allocate
                 ld   $heapFr, r1         # r1 = &heapFr
                 ld   0x0(r1), r0         # r0 = heapFr
                 add  r0, r2              # r2 = heapFr + # bytes to allocate
                 st   r2, 0x0(r1)         # heapFr += # bytes to allocate
                 j    0x0(r6)             # return old value of heapFr
.pos 0x2000
A_ping:          j    0x0(r6)             # return
A_pong:          j    0x0(r6)             # return
B_ping:          j    0x0(r6)             # return
B_wiff:          j    0x0(r6)             # return
C_ping:          deca r5                  
                 st   r6, 0x0(r5)         
                 ld   0x4(r5), r0         # r0 = this
                 ld   0x0(r0), r1         # r1 = this->class
                 ld   0x0(r1), r1         # r1 = this->class->super
                 deca r5                  
                 st   r0, 0x0(r5)         
                 gpc  $0x2, r6            
                 j    *0x4(r1)            # pc = this->class->super->ping (this)
                 inca r5                  
                 ld   0x0(r5), r6         
                 inca r5                  
                 j    0x0(r6)             
.pos 0x2200
A_class:         .long 0x0                # class definitions
                 .long A_ping
                 .long A_pong
B_class:         .long A_class
                 .long B_ping
                 .long A_pong
                 .long B_wiff
C_class:         .long B_class
                 .long C_ping
                 .long A_pong
                 .long B_wiff
.pos 0x3000
heapFr:          .long heap              # heap free pointer
.pos 0x4000
heap:            .long 0x0               # heap (snapshot)
                 .long 0x0
                 .long 0x0
                 .long 0x0
                 .long 0x0                
                 .long 0x0                
                 .long 0x0                
                 .long 0x0                
                 .long 0x0                
.pos 0x5000
                 .long 0x0                # the runtime stack
                 .long 0x0                
                 .long 0x0                
                 .long 0x0
                 .long 0x0
                 .long 0x0
                 .long 0x0
                 .long 0x0                
sb:              .long 0x0
