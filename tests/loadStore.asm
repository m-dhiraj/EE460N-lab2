.orig x3000
lea r0 neg
ldb r1 r0 #0
ldw r2 r0 #0
lea r3 forty
ldw r4 r3 #0
add r4 r4 #2
stw r4 r3 #0
halt
neg .fill xffff
forty .fill #40
.end