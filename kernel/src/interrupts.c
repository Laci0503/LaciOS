#include "../headers/interrupts.h"
#include "../headers/io.h"
#include "../headers/input.h"
#include "../headers/taskmanager.h"

void handleInterrupt(uint8 interruptnumber, CPUState currentState){
    if(interruptnumber>=HardwareInterruptOffset && interruptnumber<=HardwareInterruptOffset+0x10){
        outb(PICMasterCommand,0x20);
        if(interruptnumber>=HardwareInterruptOffset+20)outb(PICSlaveCommand,0x20);
    }
    if(interruptnumber==HardwareInterruptOffset+0){
        if(taskManagerRunning)
            currentState=schedule(currentState);
    }
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
        setInterrupt(i,8,(void (*)())((uint32)interrupts_int0+((uint32)interrupts_int1-(uint32)interrupts_int0)*i),0,0xe);
    }
    

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

    uint16 idt_pointer[3];
    idt_pointer[0]=256*8-1;
    idt_pointer[1]=((uint32) &IDT) & 0xFFFF;
    idt_pointer[2]=(((uint32) &IDT) >> 16) & 0xFFFF;

    asm volatile("lidt %0" : : "m" (idt_pointer));

    keyboard_event_handlers_slot=0;
    asm("sti");

}

/*uint8 addKeyboardEvent(void (*handler)(uint8)){
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
*/