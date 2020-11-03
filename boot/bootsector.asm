[org 0x7c00]
[bits 16]

_start:
    jmp entry

entry:

    mov ebp, 0x7FFFF
    mov esp, ebp
    
    pusha
    DETECT_MEMORY:
        mov ax, 0
        mov es, ax
        mov di, 0x500
        mov [es:di+20], dword 1
        mov ebx, 0
    DETECT_MEMORY_LOOP:
        mov edx, 0x534D4150
        mov eax, 0xE820
        mov ecx, 24
        int 0x15
        jc MEMORY_DETECT_ERROR
        test ebx, ebx
        je MEMORY_DETECT_END
        add di, 24
        jmp DETECT_MEMORY_LOOP
    MEMORY_DETECT_ERROR:
        mov si, MEMORY_DETECT_ERROR
        call PRINT_STRING_16
    MEMORY_DETECT_END:

    popa
    PCI_CHECK:
    pusha

    mov ax, 0xB101
    int 0x1a

    inc cl
    mov [0x8c1], byte cl
    mov [0x8c2], byte al
    END_PCI:
    popa

    clc
    int 0x12
    mov [0x8c3], word ax

    ;mov ah, 0
    ;mov al, 0x13
    ;int 0x10

    mov al,57
    mov bx, KERNEL
    call LOAD_FROM_DISK_16

    mov si, LOADING_MSG
    call PRINT_STRING_16

    jmp SWITCH_TO_PM
    jmp $

PRINT_STRING_16:
    mov ah, 0x0e
    mov bx, si
    PRINT_STRING_loop:
        mov al, [bx]
        inc bx
        cmp al, 0
        je PRINT_STRING_exit
        int 0x10
        jmp PRINT_STRING_loop
    PRINT_STRING_exit:
    ret

LOAD_FROM_DISK_16:
    pusha

    ;al: number of sectors
    ;bx: address

    mov ah, BIOS_READ_FROM_DISK
    mov ch, 0 ;cylinder/track
    mov dh, 0 ;read head
    mov cl, 2 ;2nd sector
    
    push bx
    mov bx, 0
    mov es, bx
    pop bx
    ;jmp $
    ;mov dl, 96
    ;mov dl, 3

    int 0x13

    jc LOAD_FROM_DISK_16_ERROR
    jmp LOAD_FROM_DISK_16_END
LOAD_FROM_DISK_16_ERROR:
    mov si, DISK_ERROR_MSG
    call PRINT_STRING_16
    jmp $
LOAD_FROM_DISK_16_END:
    popa
    ret

LOADING_MSG:
    db "Loaded kernel from drive...",10,13,0
DISK_ERROR_MSG:
    db "Diskerror",10,13,0
MEMORY_ERROR_MSG:
    db "Cant detect memory",10,13,0

GLOBAL_DESCRIPTOR_TABLE:
    GDT_NULL:
        dd 0
        dd 0
    CODE:
        dw 0xffff
        dw 0x0000
        db 0x00
        db 10011010b
        db 11001111b
        db 0x00
    DATA:
        dw 0xffff
        dw 0x0000
        db 0x00
        db 10010010b
        db 11001111b
        db 0x00
    GDT_END:
    GDT_DESCRIPTOR:
        dw GDT_END-GLOBAL_DESCRIPTOR_TABLE-1
        dd GLOBAL_DESCRIPTOR_TABLE

BIOS_READ_FROM_DISK equ 0x02

;Protected mode
SWITCH_TO_PM:
    cli ;Disable interrupts
    lgdt [GDT_DESCRIPTOR]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp (CODE-GLOBAL_DESCRIPTOR_TABLE):PM_INIT

[bits 32]
;We are in protected mode
PM_INIT:
    mov ax, DATA-GLOBAL_DESCRIPTOR_TABLE
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov bp, 0xffff
    mov sp, bp
    jmp KERNEL


[bits 16]
times 508-$+$$ db 0
MAGIC_NUMBER:
dw 0xF0F0
dw 0xaa55

[bits 32]

KERNEL: