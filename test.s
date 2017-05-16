
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


