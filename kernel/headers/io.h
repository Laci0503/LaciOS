#ifndef _IO_H
#define _IO_H
#include "types.h"

uint8 inb(uint16 port);
uint16 inb16(uint16 port);
uint32 inb32(uint16 port);
void outb(uint16 port, uint8 data);
void outb32(uint16 port, uint32 data);

#endif