.globl add
.type add, @function

add:

#push %rbp
#mov %rsp, %rbp

addl %edi, %esi

#mov %esi, %esp
#pop %rbp
movl %esi, %eax
ret
