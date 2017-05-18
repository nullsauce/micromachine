
.thumb
eor r0, r0
loop:
cmp r0, #20
beq end
mov r3, #2
mov r8, r3
add r0, #1
bx r8
end:
bl test
add r0, #5
b halt
test:
add r0, #10
halt:
svc #0


