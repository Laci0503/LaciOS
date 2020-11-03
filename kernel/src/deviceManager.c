#include "../headers/deviceManager.h"
#include "../headers/memory.h"
#include "../headers/pci.h"
#include "../headers/drives.h"
#include "../headers/ahci.h"

void deviceManagerInit(){
    SYSTEM_DEVICES=malloc(sizeof(device)*200);
    SYSTEM_DEVICE_COUNT=0;
    for(uint16 i=0;i<pciDeviceCount;i++){
        SYSTEM_DEVICES[SYSTEM_DEVICE_COUNT].bus=DEVICE_BUS_TYPE_PCI;
        SYSTEM_DEVICES[SYSTEM_DEVICE_COUNT].id=SYSTEM_DEVICE_COUNT+1;
        SYSTEM_DEVICES[SYSTEM_DEVICE_COUNT].deviceStructPointer=&pciDevices[i];
        SYSTEM_DEVICES[SYSTEM_DEVICE_COUNT].type=pciTypeResolve(pciDevices[i].class,pciDevices[i].subclass,pciDevices[i].progIf);
        SYSTEM_DEVICES[SYSTEM_DEVICE_COUNT].parent_id=0;
        SYSTEM_DEVICE_COUNT++;
    }
    for(uint16 i=0;i<SYSTEM_DEVICE_COUNT;i++){

    }
}

void setUpDevice(uint32 id){
    switch (SYSTEM_DEVICES[id].bus)
    {
    case DEVICE_BUS_TYPE_PCI:
        switch (((PCI_device*)SYSTEM_DEVICES[id].deviceStructPointer)->class)
        {
        case 0x1: // MASS STORAGE CONTROLLER
            if(((PCI_device*)SYSTEM_DEVICES[id].deviceStructPointer)->subclass == 0x1)setUpAHCI(id); // AHCI 1.0
            break;
        }
        return;
    }
    return;
}

DEVICE_TYPE pciTypeResolve(uint8 class, uint8 subclass, uint8 progIF){
    switch (class)
    {
    case 0x1:
        if(subclass==0x1)return DEVICE_TYPE_AHCI_CONTROLLER;
        break;
    case 0x2:
        if(subclass==0x0)return DEVICE_TYPE_ETHERNET;
    case 0x3:
        return DEVICE_TYPE_GRAPHICS_CARD;
    case 0x4:
        if(subclass==0x3)return DEVICE_TYPE_SOUND_CARD;
    case 0x8:
        if(subclass==0x0)return DEVICE_TYPE_PIC;
        if(subclass==0x2)return DEVICE_TYPE_TIMER;
        if(subclass==0x3)return DEVICE_TYPE_RTC;
    case 0xC:
        if(subclass==0x3 && progIF!=0xFE && progIF!=0x80)return DEVICE_TYPE_USB_HUB;
    case 0xD:
        if(subclass==0x11)return DEVICE_TYPE_BLUETOOTH;
        if(subclass==0x20 || subclass==0x21)return DEVICE_TYPE_WIRELESS_ETHERNET;
    default:
        return DEVICE_TYPE_OTHER;
    }
}