.orig x3000
and r0 r0 #0
and r1 r1 #0
and r2 r2 #0
and r3 r3 #0
lea r6 sPtr
ldw r6 r6 #0
add r0 r0 #1
add r1 r1 #3
add r2 r2 #7
add r3 r3 xf
jsr pusha
jsr pushb
jsr pushc
jsr pushd
jsr pop0
jsr pop1
jsr pop2
jsr pop3
halt
sPtr .fill x4000

pusha stw r0 r6 #0
add r6 r6 #-2
ret

pop0 add r6 r6 #2
ldw r0 r6 #0
ret

pushb stw r1 r6 #0
add r6 r6 #-2
ret

pop1 add r6 r6 #2
ldw r1 r6 #0
ret

pushc stw r2 r6 #0
add r6 r6 #-2
ret

pop2 add r6 r6 #2
ldw r2 r6 #0
ret

pushd stw r3 r6 #0
add r6 r6 #-2
ret

pop3 add r6 r6 #2
ldw r3 r6 #0
ret

.end