#include "../headers/infoPrinters.h"
#include "../headers/video.h"
#include "../headers/pci.h"
#include "../headers/ahci.h"
#include "../headers/string.h"

void printPciInfo(){
    nextLine();
    print("Number of pci buses: ");
    printint(*(uint8*)0x8c2);
    uint8 access_mechanism=*((uint8*) 0x8c1);
    nextLine();
    print("Pci access methods: ");
    if(access_mechanism & 1)print("Mechanism#1 ");
    if(access_mechanism & 2 == 2)print("Mechanism#2");
    nextLine();
}

void printPciDevices(){
    if(printPciDevices){
        print("PCI devices: ");
        nextLine();
        goxy(1, getY());
        print("Bus Slot Multif. Description                  Subclass");
        nextLine();
    }
    for(uint16 i=0;i<pciDeviceCount;i++){
        goxy(1, getY());
        printint(pciDevices[i].bus);
        goxy(5, getY());
        printint(pciDevices[i].slot);
        goxy(10, getY());
        print(pciDevices[i].multifunction ? "Yes" : "No");
        goxy(18, getY());
        if(pciDevices[i].class<0x14){
            print(pciClasses[pciDevices[i].class]);
        }else{
            print("Unknown");
        }
        goxy(47,getY());
        print(pciSubClassNames[getSubClassName(pciDevices[i].class,pciDevices[i].subclass)]);
        print(" - ");
        printint(pciDevices[i].subclass);
        nextLine();
    }
}

void printAHCI(){
    for(uint8 i=0;i<5;i++){
        if(ahci_controllers[i].implemented){
            print("AHCI controller #");
            printint(i);
            print(":");
            nextLine();
            print(" Number of ports: ");
            printint(ahci_controllers[i].ports_implemented);
            nextLine();
            print(" Number of drives: ");
            printint(ahci_controllers[i].drive_count);
            nextLine();
            print(" Version: ");
            printint(ahci_controllers[i].controller->generic_host_control.version >> 16);
            print(".");
            printint((uint16)ahci_controllers[i].controller->generic_host_control.version);
        }
    }
}