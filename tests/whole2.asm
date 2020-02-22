.orig x3100     ;cant change r0 or r2
lea r3 const
ldw r3 r3 #0
add r4 r2 #0
loop and r4 r4 r3
xor r4 r4 #-1
add r3 r3 #1
brn loop
lea r3 const
stw r4 r3 #1
add r6 r7 #0
lea r3 addr
ldw r3 r3 #0
jsrr r3
and r7 r6 #-1
ret
addr .fill x3500
const .fill xfffb
.end