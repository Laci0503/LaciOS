#include"kernel.h"
#define pci_company_start_sector 131
#define pci_company_length 20923
#define pci_company_length_sectors 41
#define pci_company_entry_count 862
#define pci_company_name_length 73

#define pci_m1_address_port 0xCF8
#define pci_m1_data_port 0xCFC

char pci_company_names[pci_company_entry_count][pci_company_name_length];
uint16 pci_company_values[pci_company_entry_count];

void load_pci_companies(char names[][pci_company_name_length], uint16 values[]){
    uint8 data[pci_company_length_sectors*512];
    readByte28bit(main_drive_bus_master,data,pci_company_start_sector,pci_company_length_sectors);
    uint8 buf[pci_company_name_length];
    uint8 status=0;
    uint8 l=0;
    uint16 index=0;
    uint8 bufferindex=0;
    uint8 tmp=1;
    for(uint8 i=0;i<pci_company_name_length;i++)buf[i]=0;
    nextLine();
    for(uint32 i =0; i<pci_company_length;i++){
        if(status){
            if (l==2 || i==pci_company_length-1){
                values[index]=(buf[1]<<8) | buf[0];
                status=0;
                for(uint8 j=0;j<pci_company_name_length;j++)buf[j]=0;
                bufferindex=1;
                l=0;
                buf[0]=data[i];
                index++;
            }else{
                l++;
                buf[bufferindex]=data[i];
                bufferindex++;
            }
        }else{
            if (data[i]!=0){
                buf[bufferindex]=data[i];
                bufferindex++;
            }else{
                for(uint8 j = 0;j<bufferindex;j++){
                    names[index][j]=buf[j];
                }
                status=1;
                for(uint8 j=0;j<pci_company_name_length;j++)buf[j]=0;
                bufferindex=0;
            }
        }
    }
    return;
}
uint16 pciRead16(uint8 bus, uint8 slot, uint8 function, uint8 offset){
    uint8 access_mechanism=*((uint8*) 0x8c1);
    if(access_mechanism & 1){
        uint32 s = (uint32)slot;
        uint32 b = (uint32)bus;
        uint32 f = (uint32)function;
        uint32 o = (uint32)offset;

        uint32 address = (b << 16) | (s << 11) |
              (f << 8) | (o & 0xfc) | 0x80000000;

        outb32(pci_m1_address_port, address);
        uint32 input=inb32(pci_m1_data_port);
        //printint(input);
        uint16 output = (uint16)((input >> ((offset & 2) * 8)) & 0xffff);

        return output;
    }
}

uint32 pciRead32(uint8 bus, uint8 slot, uint8 function, uint8 offset){
    uint8 access_mechanism=*((uint8*) 0x8c1);
    if(access_mechanism & 1){
        uint32 s = (uint32)slot;
        uint32 b = (uint32)bus;
        uint32 f = (uint32)function;
        uint32 o = (uint32)offset;

        uint32 address = (b << 16) | (s << 11) |
              (f << 8) | (o & 0xfc) | 0x80000000;

        outb32(pci_m1_address_port, address);
        uint32 input=inb32(pci_m1_data_port);
        return input;
    }
}


void pciWrite16(uint8 bus, uint8 slot, uint8 function, uint8 offset, uint16 data){
    uint8 access_mechanism=*((uint8*) 0x8c1);
    if(access_mechanism & 1){
        uint32 s = (uint32)slot;
        uint32 b = (uint32)bus;
        uint32 f = (uint32)function;
        uint32 o = (uint32)offset;

        uint32 address = (b << 16) | (s << 11) |
              (f << 8) | (o & 0xfc) | 0x80000000;

        outb32(pci_m1_address_port, address);
        uint32 input=inb32(pci_m1_data_port);
        outb32(pci_m1_address_port, address);

        if((offset & 2) == 0){
            input &= 0x00FF;
            outb32(pci_m1_data_port,input | data);
        }else{
            input &= 0xFF00;
            outb32(pci_m1_data_port,input | (data << 8));
        }
        return;
    }
}

void pciWrite32(uint8 bus, uint8 slot, uint8 function, uint8 offset, uint32 data){
    uint8 access_mechanism=*((uint8*) 0x8c1);
    if(access_mechanism & 1){
        uint32 s = (uint32)slot;
        uint32 b = (uint32)bus;
        uint32 f = (uint32)function;
        uint32 o = (uint32)offset;

        uint32 address = (b << 16) | (s << 11) |
              (f << 8) | (o & 0xfc) | 0x80000000;

        outb32(pci_m1_address_port, address);
        outb32(pci_m1_data_port,data);
        return;
    }
}

uint16 getVendorNumber(uint8 bus, uint8 slot){
    return pciRead16(bus,slot,0,0);
}

void getVendorName(uint16 vendorId, char buffer[]){
    for(uint16 i = 0;i<pci_company_entry_count;i++){
        if(pci_company_values[i]==vendorId){
            for(uint8 j=0;j<pci_company_name_length;j++){
                buffer[j]=pci_company_names[i][j];
            }
            return;
        }
    }
    char b[] ="Not found";
    for(uint8 j=0;j<10;j++){
        buffer[j]=b[j];
    }
}

uint8 getHeaderType(uint8 bus, uint8 slot){
    return (uint8)(pciRead16(bus,slot,0,14) & 0xFF);
}

uint8 isMultiFunction(uint8 bus, uint8 slot){
    return getHeaderType(bus,slot) & 0x80 == 0x80;
}

uint8 getClass(uint8 bus, uint8 slot, uint8 function){
    return (uint8)(pciRead16(bus,slot,function,10) >> 8);
}
uint8 getSubClass(uint8 bus, uint8 slot, uint8 function){
    return (uint8)(pciRead16(bus,slot,function,10) & 0x00FF);
}
uint16 getProgIF(uint8 bus, uint8 slot, uint8 function){
    return (uint8)(pciRead16(bus,slot,function,0x8) >> 8);
}

char pciSubClassNames[][30] = {"SCSI Bus controller",
    "IDE controller",
    "ATA controller",
    "Serial ATA",
    "Ethernet adapter",
    "VGA Compatible controller",
    "XGA Compatible adapter",
    "3D Controller",
    "USB Controller",
    "Ethernet Controller (802.1a)",
    "Ethernet Controller (802.1b)",
    "PCI-to-PCI bridge",
    "Other"
};

uint8 getSubClassName(uint8 class, uint8 subClass){
    if(class==0x1){
        switch(subClass){
            case (0x0) : return 0;
            case (0x1) : return 1;
            case (0x5) : return 2;
            case (0x6) : return 3;
        }
    }else if(class==0x2){
        if(subClass==0x0)return 4;
    }else if(class==0x3){
        switch(subClass){
            case (0x0) : return 5;
            case (0x1) : return 6;
            case (0x5) : return 7;
        }
    }else if(class==0x6){
        if(subClass==0x4 || subClass==0x9){
            return 11;
        }
    }else if(class==0x0C){
        if(subClass==0x3){
            return 8;
        }
    }else if(class==0x0D){
        switch(subClass){
            case (0x20) : return 9;
            case (0x21) : return 10;
        }
    }
    return 12;
}

uint32 getBaseAddress(struct PCI_device* pci_device, uint8 barid){
    uint8 header_type=getHeaderType((*pci_device).bus,(*pci_device).slot);
    if((header_type & 0b01111111)==0){
        return (pciRead32((*pci_device).bus,(*pci_device).slot,0,0x10+barid*4));
    }
    return 0;
}

void mapPci(uint8 printTable){
    char buffer[pci_company_name_length];
    for(uint8 i=0;i<pci_company_name_length;i++)buffer[i]=0;

    uint8 printPciDevices=printTable;
    
    if(printPciDevices){
        print("PCI devices: ");
        nextLine();
        goxy(1, getY());
        print("Bus Slot Multif. Description                  Subclass");
        nextLine();
    }

    uint16 vendorID;
    uint8 classNumber;
    char test[]="asdasd";
    uint8 ahci_controller_index=0;
    for(uint8 i = 0;i<5;i++)ahci_controllers[i].implemented=0x0;
    for(uint8 j = 0;j<*(uint8*)0x8c1;j++){
        for(uint8 k = 0;k<32;k++){
            vendorID=getVendorNumber(j,k);
            classNumber=getClass(j,k,0);
            if(vendorID!=0xFFFF){
                if(printPciDevices){
                    goxy(1, getY());
                    print("0 ");
                    goxy(5, getY());
                    printint(k);
                    goxy(10, getY());
                    print(isMultiFunction(j,k) ? "Yes" : "No");
                    goxy(18, getY());
                    if(classNumber<0x14){
                        print(pciClasses[classNumber]);
                    }else{
                        print("Unknown");
                    }
                    goxy(47,getY());
                    print(pciSubClassNames[getSubClassName(classNumber,getSubClass(j,k,0))]);
                    print(" - ");
                    printint(getSubClass(j,k,0));
                    nextLine();
                }
                if(classNumber==0x1 && getSubClass(j,k,0)==0x6 && getProgIF(j,k,0)==0x1){
                    ahci_controllers[ahci_controller_index].implemented=1;
                    ahci_controllers[ahci_controller_index].pci_device.bus=j;
                    ahci_controllers[ahci_controller_index].pci_device.slot=k;
                    ahci_controllers[ahci_controller_index].pci_device.function=0;
                    ahci_controllers[ahci_controller_index].controller=(struct AHCI_controller*)getBaseAddress(&ahci_controllers[ahci_controller_index].pci_device,5);
                    ahci_controller_index++;
                }

            }
        }
    }
}

void printPciInfo(){
    nextLine();
    print("Number of pci buses: ");
    printint(*(uint8*)0x8c1);
    uint8 access_mechanism=*((uint8*) 0x8c1);
    nextLine();
    print("Pci access methods: ");
    if(access_mechanism & 1)print("Mechanism#1 ");
    if(access_mechanism & 2 == 2)print("Mechanism#2");
}