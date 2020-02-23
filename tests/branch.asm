.orig x3000
and r0 r0 #0
and r1 r1 #0
add r0 r0 #-4
loop add r1 r1 #5
add r0 r0 #1
brnz loop
lea r0 store
stb r1 r0 #0
halt
store .fill #10
.end