#include "kernel.h"

typedef volatile struct IDT_entry{
    uint16 address_low;
    uint16 selector;
    uint8 zero;
    uint8 attributes;
    uint16 address_high;

};
typedef volatile struct IDT{
    struct IDT_entry entries[256];
};
typedef volatile struct IDT_descriptor{
    uint16 length; //256*8
    uint32 base_address;
};

struct IDT global_idt;
struct IDT_descriptor global_idt_descriptor;
extern void handle_interrupt0();
extern void handle_interrupt1();

typedef void func(void);

void IDT_init(){
    global_idt_descriptor.base_address=(uint32)&global_idt;
    global_idt_descriptor.length=256*8 - 1;

    for(uint8 i = 0;i<32;i++){
        global_idt.entries[i].address_low=(uint32)(&handle_interrupt0 + i*(&handle_interrupt1-&handle_interrupt0)) & 0xFFFF;
        global_idt.entries[i].address_high=((uint32)(&handle_interrupt0 + i*(&handle_interrupt1-&handle_interrupt0)) >> 16) & 0xFFFF;
        global_idt.entries[i].zero=0;
        global_idt.entries[i].attributes=0x8F;
        global_idt.entries[i].selector=8;
        //printhex((uint32)(global_idt.entries[i].address_high << 16) | global_idt.entries[i].address_low);
        //printhex();
        //nextLine();
    }
    for(uint16 i = 32;i<256;i++){
        global_idt.entries[i].address_low=(uint32)(&handle_interrupt0 + i*(&handle_interrupt1-&handle_interrupt0)) & 0xFFFF;
        global_idt.entries[i].address_high=((uint32)(&handle_interrupt0 + i*(&handle_interrupt1-&handle_interrupt0)) >> 16) & 0xFFFF;
        global_idt.entries[i].zero=0;
        global_idt.entries[i].attributes=0x8E;
        global_idt.entries[i].selector=8;
    }

    struct IDT* test = (struct IDT*)global_idt_descriptor.base_address;

    //func* asd = (func*)0xBB20;
    //printhex(test->entries[0].address_low);
    //asd();
    //while(1);

    //printbin(global_idt.entries[0].attributes);

    printhex(&handle_interrupt1-&handle_interrupt0);

    //while(1);

    /*clear();
    goxy(0,0);

    uint16* pointer = &global_idt;

    for(uint8 i=0;i<25;i++){
        for(uint8 j=0;j<4;j++){
            goxy(j*(WIDTH/4),i);
            printhex(*pointer);
            pointer++;
        }
    }*/

    //while(1);

    outb(0x20,0x11);
    outb(0xA0,0x11);

    outb(0x20,0x20);
    outb(0xA0,0x28);

    outb(0x20,0x4);
    outb(0xA0,0x2);

    outb(0x20,0x1);
    outb(0xA0,0x1);

    outb(0x20,0x0);
    outb(0xA0,0x0);

    asm("lidt %0" : : "m" (global_idt_descriptor));
    //while(1);
    asm volatile("sti");
    return;
}

uint32 kernel_handle_interrupt(uint32 interrupt_number,uint32 esp){
    printint(interrupt_number);
    //while(1);
    return esp;
}

uint32 test(uint32 num){
    return num;
}