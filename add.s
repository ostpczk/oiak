.section .data

.lcomm pointer_1, 4 # wskaznik na o_1
.lcomm pointer_2, 4 # wskaznik na o_2
.lcomm output, 64

.section .text
.globl add
.type add, @function

# FUNKCJA ADD
# arg. 1 - wskaznik na liczbe 1
# arg. 2- wskaznik na liczbe 2
# wartosc zwracana - suma obu tych liczb

add:
pushl %ebp
movl %esp, %ebp

movl 8(%ebp), %eax # wskaznik na o_1
movl %eax, pointer_1
movl 4(%ebp), %eax # wskaznik na o_2 
movl %eax, pointer_2

movb pointer_1, %bl #
movb pointer_2, %cl # ten kod obecnie nie robi niczego uzytecznego.
xorb %bl, %dl       # myslimy nad implementacja uzytecznych funkcjonalnosci.
movb %dl, output    # to ma obliczac pierwszy bit - bit znaku
                    # prosze byc ostroznym z rejestrem ecx! printf go potrzebuje!

# w %eax znajduje sie wartosc zwracana
movl $output, %eax

xor %ebx, %ebx
xor %edx, %edx

movl %ebp, %esp
popl %ebp
ret
