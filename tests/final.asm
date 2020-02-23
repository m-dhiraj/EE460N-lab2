.orig x3000

;two loops
;outer one will increase loc
;inner one counts by 4
;r0 is address
;r1 is counter1
;r2 is counter2
;r3 is data
;r4 is what to add

lea r0 loc
add r0 r0 #3
and r1 r1 #0
and r2 r2 #0
and r3 r3 #0
and r4 r4 #0
and r5 r5 #0
add r1 r1 x4

loop0 add r5 r5 #2
loop1 add r2 r2 x2
loop2 add r4 r4 #1
lshf r3 r3 #4
add r3 r3 r4
add r2 r2 #-1
brp loop2
stb r3 r0 #0
add r0 r0 #-1
add r5 r5 #-1
brp loop1
add r0 r0 #4
add r1 r1 #-1
brp loop0

halt

loc .fill #0

.end