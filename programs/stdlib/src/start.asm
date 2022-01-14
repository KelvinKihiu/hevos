[BITS 32]

global _start
extern c_start
extern hevos_exit

section .asm

_start:
    call c_start
    push eax
    call hevos_exit
    add esp, 4
    ret