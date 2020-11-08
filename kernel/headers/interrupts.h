#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H
#include "types.h"

#define HardwareInterruptOffset 0x20
#define PICMasterCommand 0x20
#define PICMasterData 0x21
#define PICSlaveCommand 0xA0
#define PICSlaveData 0xA1

typedef volatile struct{
    uint32 eax;
    uint32 ebx;
    uint32 ecx;
    uint32 edx;

    uint32 esi;
    uint32 edi;
    uint32 ebp;

    uint32 eip;
    uint32 esp;
    uint32 cs;
} CPUState;

typedef volatile struct
{
    int16 handlerAddressLowBits;
    uint16 gdt_codeSegmentSelector;
    uint8 reserved;
    uint8 access;
    uint16 handlerAddressHighBits;
} GateDescriptor;
typedef volatile struct{
    uint16 size;
    uint32 base;
} IDT_descriptor;

GateDescriptor IDT[256];
uint16 keyboard_event_handlers_slot;
void (*keyboard_event_handlers[16])(uint8);

uint32 handleInterrupt(uint8 interruptnumber, uint32 esp);
void setInterrupt(uint8 interrupt,uint16 codeSegmentSelectorOffset, void (*handler)(),uint8 DescriptorPrivilegeLevel, uint8 DescriptorType);
void setupInterrupts();
uint8 addKeyboardEvent(void (*handler)(uint8));
void removeKeyboardEvent(uint8 slot);
void keystroke_ignore(uint8 keycode);

extern void InterruptIgnore();
extern void HandleInterruptRequest0x00();
extern void HandleInterruptRequest0x01();
extern void HandleInterruptRequest0x02();
extern void HandleInterruptRequest0x03();
extern void HandleInterruptRequest0x04();
extern void HandleInterruptRequest0x05();
extern void HandleInterruptRequest0x06();
extern void HandleInterruptRequest0x07();
extern void HandleInterruptRequest0x08();
extern void HandleInterruptRequest0x09();
extern void HandleInterruptRequest0x0a();
extern void HandleInterruptRequest0x0b();
extern void HandleInterruptRequest0x0c();
extern void HandleInterruptRequest0x0d();
extern void HandleInterruptRequest0x0e();
extern void HandleInterruptRequest0x0f();

#endif