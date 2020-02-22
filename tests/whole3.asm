.orig x3500 ;cant change r0 r2 r6 r7
lea r3 data
ldw r4 r3 #0
lshf r4 r4 #1
stw r4 r3 #2
ret
data .fill #1
.end