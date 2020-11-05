#include "../headers/interrupts.h"
#include "../headers/io.h"
#include "../headers/input.h"

uint32 handleInterrupt(uint8 interruptnumber, uint32 esp){
    if(interruptnumber>=HardwareInterruptOffset && interruptnumber<=HardwareInterruptOffset+0x10){
        outb(PICMasterCommand,0x20);
        if(interruptnumber>=HardwareInterruptOffset+20)outb(PICSlaveCommand,0x20);
    }
    if(interruptnumber==HardwareInterruptOffset+1){
        for(uint8 i=0;i<16;i++){
            (*keyboard_event_handlers[i])(next_keyboard_byte());
        }
    }else if(interruptnumber==HardwareInterruptOffset+0){ //Timer interrupt
        
    }
    return esp;
}

void setInterrupt(uint8 interrupt,uint16 codeSegmentSelectorOffset, void (*handler)(),uint8 DescriptorPrivilegeLevel, uint8 DescriptorType){
    IDT[interrupt].handlerAddressLowBits=((uint32) handler) & 0xFFFF;
    IDT[interrupt].handlerAddressHighBits=(((uint32) handler) >> 16) & 0xFFFF;
    IDT[interrupt].gdt_codeSegmentSelector=8;
    IDT[interrupt].access=0x80 | ((DescriptorPrivilegeLevel & 3) << 5) | DescriptorType;
    IDT[interrupt].reserved=0;
}

void setupInterrupts(){
    for(uint16 i=0;i<256;i++){
        setInterrupt(i,8,InterruptIgnore,0,0xe);
    }

    setInterrupt(HardwareInterruptOffset+0,8,HandleInterruptRequest0x00,0,0xe);
    setInterrupt(HardwareInterruptOffset+1,8,HandleInterruptRequest0x01,0,0xe);
    setInterrupt(HardwareInterruptOffset+2,8,HandleInterruptRequest0x02,0,0xe);
    setInterrupt(HardwareInterruptOffset+3,8,HandleInterruptRequest0x03,0,0xe);
    setInterrupt(HardwareInterruptOffset+4,8,HandleInterruptRequest0x04,0,0xe);
    setInterrupt(HardwareInterruptOffset+5,8,HandleInterruptRequest0x05,0,0xe);
    setInterrupt(HardwareInterruptOffset+6,8,HandleInterruptRequest0x06,0,0xe);
    setInterrupt(HardwareInterruptOffset+7,8,HandleInterruptRequest0x07,0,0xe);
    setInterrupt(HardwareInterruptOffset+8,8,HandleInterruptRequest0x08,0,0xe);
    setInterrupt(HardwareInterruptOffset+9,8,HandleInterruptRequest0x09,0,0xe);
    setInterrupt(HardwareInterruptOffset+10,8,HandleInterruptRequest0x0a,0,0xe);
    setInterrupt(HardwareInterruptOffset+11,8,HandleInterruptRequest0x0b,0,0xe);
    setInterrupt(HardwareInterruptOffset+12,8,HandleInterruptRequest0x0c,0,0xe);
    setInterrupt(HardwareInterruptOffset+13,8,HandleInterruptRequest0x0d,0,0xe);
    setInterrupt(HardwareInterruptOffset+14,8,HandleInterruptRequest0x0e,0,0xe);
    setInterrupt(HardwareInterruptOffset+15,8,HandleInterruptRequest0x0f,0,0xe);
    

    outb(PICMasterCommand,0x11);
    outb(PICSlaveCommand,0x11);

    outb(PICMasterData,HardwareInterruptOffset);
    outb(PICSlaveData,HardwareInterruptOffset);

    outb(PICMasterData,0x4);
    outb(PICSlaveData,0x2);

    outb(PICMasterData,0x1);
    outb(PICSlaveData,0x1);

    outb(PICMasterData,0x0);
    outb(PICSlaveData,0x0);

    /*struct IDT_descriptor idt_pointer;
    idt_pointer.size=256*8-1;
    idt_pointer.base=0xffffffff;
    printhex(idt_pointer.base);
    nextLine();
    printhex(&IDT);*/

    uint16 idt_pointer[3];
    idt_pointer[0]=256*8-1;
    idt_pointer[1]=((uint32) &IDT) & 0xFFFF;
    idt_pointer[2]=(((uint32) &IDT) >> 16) & 0xFFFF;

    asm volatile("lidt %0" : : "m" (idt_pointer));
    //while(1);

    for(uint8 i=0;i<16;i++){
        keyboard_event_handlers[i]=keystroke_ignore;
    }
    keyboard_event_handlers_slot=0;

    asm("sti");

}

uint8 addKeyboardEvent(void (*handler)(uint8)){
    for(uint8 i=0;i<16;i++){
        if((keyboard_event_handlers_slot & 0x1 << i)==0){
            keyboard_event_handlers[i]=handler;
            keyboard_event_handlers_slot |= 0x0 | (0x1 << i);
            return i;
        }
    }
}
void removeKeyboardEvent(uint8 slot){
    keyboard_event_handlers[slot]=keystroke_ignore;
    keyboard_event_handlers_slot &= 0xFFFF & (0x1 << slot);
}

void keystroke_ignore(uint8 keycode){return;}