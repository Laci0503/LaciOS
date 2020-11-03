.set IRQ_BASE, 0x20

.section .text

.extern handleInterrupt


.macro HandleException num
.global HandleException\num\()Ev
HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm


.macro HandleInterruptRequest num
.global HandleInterruptRequest\num\()
HandleInterruptRequest\num\():
    movb $\num + IRQ_BASE, (interruptnumber)
    pushl $0
    jmp int_bottom
.endm


HandleException 0x00
HandleException 0x01
HandleException 0x02
HandleException 0x03
HandleException 0x04
HandleException 0x05
HandleException 0x06
HandleException 0x07
HandleException 0x08
HandleException 0x09
HandleException 0x0a
HandleException 0x0b
HandleException 0x0c
HandleException 0x0d
HandleException 0x0e
HandleException 0x0f
HandleException 0x10
HandleException 0x11
HandleException 0x12
HandleException 0x13

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x02
HandleInterruptRequest 0x03
HandleInterruptRequest 0x04
HandleInterruptRequest 0x05
HandleInterruptRequest 0x06
HandleInterruptRequest 0x07
HandleInterruptRequest 0x08
HandleInterruptRequest 0x09
HandleInterruptRequest 0x0a
HandleInterruptRequest 0x0b
HandleInterruptRequest 0x0c
HandleInterruptRequest 0x0d
HandleInterruptRequest 0x0e
HandleInterruptRequest 0x0f
HandleInterruptRequest 0x31

HandleInterruptRequest 0x80


int_bottom:

    # save registers
    #pusha
    #pushl %ds
    #pushl %es
    #pushl %fs
    #pushl %gs
    
    pushl %ebp
    pushl %edi
    pushl %esi

    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax

    # load ring 0 segment register
    #cld
    #mov $0x10, %eax
    #mov %eax, %eds
    #mov %eax, %ees

    # call C++ Handler
    pushl %esp
    push (interruptnumber)
    call handleInterrupt
    #add %esp, 6
    mov %eax, %esp # switch the stack

    # restore registers
    popl %eax
    popl %ebx
    popl %ecx
    popl %edx

    popl %esi
    popl %edi
    popl %ebp
    #pop %gs
    #pop %fs
    #pop %es
    #pop %ds
    #popa
    
    add $4, %esp

.global InterruptIgnore
InterruptIgnore:

    iret


.data
    interruptnumber: .byte 0
