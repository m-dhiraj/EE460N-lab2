.ORIG	x3000
     lea r0 loc
     ldw r1 r0 #0
     ldw r3 r0 #1
     xor r2 r1 #-1
     xor r4 r1 r3
     stw r2 r0 #2
     stw r4 r0 #3
     HALT
loc .FILL	x00ff
     .fill x0ff0
.END