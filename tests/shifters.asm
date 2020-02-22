.orig x3000
        lea r0 data     ;r0 is the address
        and r1 r1 #0    ;r1 is the counter
        add r1 r1 #10
        ldw r2 r0 #0    ;r2 is the data
loop    add r0 r0 x2
        rshfa r2 r2 #1
        stw r2 r0 #0
        add r1 r1 #-1
        brp loop
        halt
data    .fill xf300
.end