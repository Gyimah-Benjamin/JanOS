bits 32

global keyboard_asm
global timer_asm
global dum_asm
extern timer_handler
extern get_key

keyboard_asm:
    pusha
    call get_key
    mov al, 0x20
    out 0x20, al ; send AL to port 0x20 (end of interrupt)
    popa
    iret

timer_asm:
    pusha
    call timer_handler
    mov al, 0x20
    out 0x20, al
    popa
    iret
dum_asm:
    pusha
    mov al, 0x20
    out 0x20, al
    popa
    iret
