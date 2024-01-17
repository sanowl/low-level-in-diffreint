section .data
    helloMessage db 'Hello, Assembly World!',0Ah  ; message to be printed
    len equ $ - helloMessage                    ; length of the message

section .bss
    num resb 1                                  ; reserve one byte for number

section .text
    global _start

_start:
    ; Print Hello Message
    mov eax, 4              ; syscall number for sys_write
    mov ebx, 1              ; file descriptor 1 is stdout
    mov ecx, helloMessage   ; pointer to message to write
    mov edx, len            ; message length
    int 0x80                ; call kernel

    ; Simple Arithmetic Operation (Addition)
    mov al, 5               ; load 5 into al
    add al, 3               ; add 3 to al
    mov [num], al           ; store the result in num

    ; Exiting the program
    mov eax, 1              ; syscall number for sys_exit
    xor ebx, ebx            ; exit code 0
    int 0x80                ; call kernel
