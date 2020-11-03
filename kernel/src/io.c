#include "../headers/io.h"

uint8 inb(uint16 port){
    uint8 ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
    return ret;
}

uint16 inb16(uint16 port){
    uint16 ret;
    __asm__ volatile("inw %1, %0" : "=a" (ret) : "Nd" (port));
    return ret;
}

uint32 inb32(uint16 port){
    uint32 ret = 0;
    asm volatile("inl %1, %0" : "=a"(ret) : "d"(port));
    return ret;
}

void outb(uint16 port, uint8 data)
{
  asm volatile("outb %0, %1" : : "a" (data), "Nd" (port));
  return;
}

void outb32(uint16 port, uint32 data){
    asm volatile("outl %0, %1" : : "a"(data), "Nd" (port));
}