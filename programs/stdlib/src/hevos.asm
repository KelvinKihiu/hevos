[BITS 32]

section .asm

global print:function
global hevos_getkey:function
global hevos_malloc:function
global hevos_free:function
global hevos_putchar:function
global hevos_process_load_start:function
global hevos_process_get_arguments:function
global hevos_system:function
global hevos_exit:function


; void hevos_exit(int err_code)
hevos_exit:
    push ebp
    mov ebp, esp
    push dword[ebp+8] ; Push the error code
    mov eax, 0
    int 0x80
    add esp, 4
    pop ebp
    ret

; void print(const char* message);
print:
    push ebp
    mov ebp, esp
    push dword[ebp+8]
    mov eax, 1
    int 0x80
    add esp, 4
    pop ebp
    ret

; int hevos_getkey();
hevos_getkey:
    push ebp
    mov ebp, esp
    mov eax, 2
    int 0x80
    pop ebp
    ret

; void hevos_putchar(char c)
hevos_putchar:
    push ebp
    mov ebp, esp
    push dword [ebp+8]
    mov eax, 3
    int 0x80
    add esp, 4
    pop ebp
    ret

; void* hevos_malloc(size_t size)
hevos_malloc:
    push ebp
    mov ebp, esp
    mov eax, 4
    push dword[ebp+8]
    int 0x80
    add esp, 4
    pop ebp
    ret


; void hevos_free(void* size)
hevos_free:
    push ebp
    mov ebp, esp
    mov eax, 5
    push dword[ebp+8]
    int 0x80
    add esp, 4
    pop ebp
    ret


; void hevos_process_load_start(const char* filename)
hevos_process_load_start:
    push ebp
    mov ebp, esp
    mov eax, 6
    push dword[ebp+8]
    int 0x80
    add esp, 4
    pop ebp
    ret


; int hevos_system(struct command_argument* argument)
hevos_system:
    push ebp
    mov ebp, esp
    mov eax, 7
    push dword[ebp+8]
    int 0x80
    add esp, 4
    pop ebp
    ret


; void hevos_process_get_arguments(struct process_arguments* arguments)
hevos_process_get_arguments:
    push ebp
    mov ebp, esp
    mov eax, 8
    push dword[ebp+8]
    int 0x80
    add esp, 4
    pop ebp
    ret