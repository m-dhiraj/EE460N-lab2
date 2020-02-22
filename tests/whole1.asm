.orig x3000
lea r1 addr     ;r1 is the address
ldw r2 r1 #1    ;r2 is the data
ldw r1 r1 #0
and r0 r0 #0    ;r0 is counter init -8
add r0 r0 #-8
loop lea r1 addr
ldw r1 r1 #0
jsrr r1         ;run subroutine
add r2 r2 #1    ;increment data
add r0 r0 #2    ;decrement counter
brnz loop
lea r0 addr     ;store data
stw r2 r0 #1
halt
addr .fill x3100
.fill x2468
.end