bits 16
org 0x7C00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov si, dap
    mov ah, 0x42
    mov dl, 0x80
    int 0x13
    jc disk_error

    mov si, disk_dap
    mov ah, 0x42
    mov dl, 0x80
    int 0x13
    jc disk_error

    jmp continue_boot

disk_error:
    mov ah, 0x0E
    mov al, 'E'
    int 0x10
    hlt

continue_boot:
    xor ax, ax
    mov es, ax

    in al, 0x92
    or al, 2
    out 0x92, al

    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:protected_mode

dap:
    db 0x10
    db 0
    dw 30
    dw 0x0000
    dw 0x0100
    dq 100

disk_dap:
    db 0x10
    db 0
    dw 60
    dw 0x0000
    dw 0x0900
    dq 0

gdt_start:
    dq 0
gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00
gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

bits 32
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov esp, 0x90000

    call 0x1000

    cli
    hlt

times 510-($-$$) db 0
dw 0xAA55
