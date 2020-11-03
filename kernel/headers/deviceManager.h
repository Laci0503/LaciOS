#ifndef _DEVICE_MANAGER_H
#define _DEVICE_MANAGER_H
#include "types.h"

typedef enum{
    DEVICE_BUS_TYPE_PCI,
    DEVICE_BUS_TYPE_USB,
    DEVICE_BUS_TYPE_ATA,
    DEVICE_BUS_TYPE_AHCI,
    DEVICE_BUS_TYPE_UNKNOWN = 0xFE,
    DEVICE_BUS_TYPE_OTHER   = 0xFF
} DEVICE_BUS_TYPE;

typedef enum{
    DEVICE_TYPE_DRIVE,
    DEVICE_TYPE_KEYBOARD,
    DEVICE_TYPE_MOUSE,
    DEVICE_TYPE_GRAPHICS_CARD,
    DEVICE_TYPE_USB_STORAGE,
    DEVICE_TYPE_USB_HUB,
    DEVICE_TYPE_AHCI_CONTROLLER,
    DEVICE_TYPE_SOUND_CARD,
    DEVICE_TYPE_ETHERNET,
    DEVICE_TYPE_WIRELESS_ETHERNET,
    DEVICE_TYPE_BLUETOOTH,
    DEVICE_TYPE_PIC,
    DEVICE_TYPE_TIMER,
    DEVICE_TYPE_RTC,

    DEVICE_TYPE_UNKNOWN = 0xFFFE,
    DEVICE_TYPE_OTHER = 0xFFFF
} DEVICE_TYPE;

typedef struct{
    DEVICE_BUS_TYPE bus;
    DEVICE_TYPE type;
    uint32 id;
    void* deviceStructPointer;
    uint32 parent_id
    
} device;

uint16 SYSTEM_DEVICE_COUNT;

device* SYSTEM_DEVICES;

DEVICE_TYPE pciTypeResolve(uint8 class, uint8 subclass, uint8 progIF);
void setUpDevice(uint32 id);
void deviceManagerInit();

#endif