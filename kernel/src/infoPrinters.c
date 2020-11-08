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

void print_memory_map(){
    print("Aviable ram: ");
    printint(heap_size/(1024*1024));
    print(" MiB");
    nextLine();

    print("Memory map:");

    nextLine();
    goxy(1,getY());
    print("Base address");
    goxy(26,getY());
    print("Length");
    goxy(52,getY());
    print("Type");
    nextLine();
    
    uint8 cl=getY();

    for(uint8 i = 0;i<memory_map_region_count;i++){
        if(memory_map_region_type[i]==1){forecolor=LIGHT_GREEN;}else{forecolor=LIGHT_YELLOW;}
        goxy(1,getY());
        print("0x");
        printhex(memroy_map_region_base[i]);
        goxy(26,getY());
        print("0x");
        printhex(memory_map_region_length[i]);
        goxy(52,getY());
        if(memory_map_region_type[i]==1){
            print("Usable ram");
        }else if(memory_map_region_type[i]==2){
            print("Reserved");
        }else if(memory_map_region_type[i]==3){
            print("ACPI reclaimable memory");
        }else if(memory_map_region_type[i]==4){
            print("ACPI NVS memory");
        }else if(memory_map_region_type[i]==5){
            print("Bad memory");
        }
        nextLine();
    }
    forecolor=WHITE;

}

void printMemoryManagerTable(){
    nextLine();
    print("Memory manager table");
    nextLine();
    print(" Base          State");
    nextLine();
    for(uint8 i=0;i<memory_piece_count;i++){
        print(" 0x");
        printhex(memory_pieces[i]);
        goxy(15,getY());
        if(mem_get_state(i))print("Free"); else print("Reserved");
        nextLine();
    }
}

void mmap_terminal_command(string_array* args){
    print_memory_map();
    return;
}
void memory_manager_terminal_command(string_array* args){
    if(args->length==0){
        printMemoryManagerTable();
    }
}