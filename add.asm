SYSCALL32 = 0x80
SYSEXIT = 1

.globl add
.type add, @function

add:

push exp
mov %esp, %ebp

addl %edi, %esi

mov %esi, %esp
pop %ebp
ret

mov $SYSEXIT, %eax
mov $0, %ebx
int $SYSCALL32


