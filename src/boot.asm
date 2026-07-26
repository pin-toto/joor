[bits 32]
[global _start]

section .multiboot
    align 4
    dd 0x1BADB002
    dd 0x03
    dd -(0x1BADB002 + 0x03)

section .text
_start:
    mov esp, stack_top
    cli
    extern kernel_main
    call kernel_main
    cli
    hlt
    jmp $

section .bss
    align 16
    stack_bottom:
        resb 16384
    stack_top:
