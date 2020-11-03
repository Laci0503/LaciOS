/*#include "../headers/ata.h"
#include "../headers/drives.h"
#include "../headers/ahci.h"*/
#include "../headers/types.h"
#include "../headers/io.h"

#define LOOK_FOR_ATA 1
#define LOOK_FOR_PCI_STORAGE 0
#define KERNEL_START_SECTOR 58
#define KERNEL_SECTOR_COUNT 120
#define pci_m1_address_port 0xCF8
#define pci_m1_data_port 0xCFC

void ata_identify(uint8 bus_master,uint64 result[]);
void ata_readByte28bit(uint8 bus_master,uint8 result[], uint32 lba, uint8 sector_count);
uint16 pciRead16(uint8 bus, uint8 slot, uint8 function, uint8 offset);
uint32 pciRead32(uint8 bus, uint8 slot, uint8 function, uint8 offset);
uint16 getDeviceId(uint8 bus, uint8 slot, uint8 function);
uint16 getProgIF(uint8 bus, uint8 slot, uint8 function);
uint8 getSubClass(uint8 bus, uint8 slot, uint8 function);
uint8 getClass(uint8 bus, uint8 slot, uint8 function);
uint8 isMultiFunction(uint8 bus, uint8 slot);
uint8 getHeaderType(uint8 bus, uint8 slot);
void checkPciForStorage(uint8 bus,uint8 slot,uint8 function);
uint16 getVendorNumber(uint8 bus, uint8 slot, uint8 function);
uint32 getBaseAddress(uint8 bus,uint8 slot,uint8 function, uint8 barid);

void _KERNEL_ENTRY(){
    for(uint16 i=0;i<80*25;i++)((uint16*)0xB8000)[i]=0;

    #if (LOOK_FOR_ATA==1)
        uint64 result[9];
        for(uint8 i=0;i<9;i++)result[i]=0;
        uint8 result2[512];

        uint8 bus_master=0b10000000;
        ata_identify(bus_master,result);
        if(result[0]==0 && result[7]!=0){
            ata_readByte28bit(bus_master,result2,0,1);
            if(result2[0x1FC]==0xF0&&result2[0x1FD]==0xF0&&result2[0x1FE]==0x55&&result2[0x1FF]==0xAA){
                ata_readByte28bit(bus_master,(uint8*)0x100000,58,120);
                //ata_readByte28bit(bus_master,(uint8*)(0x100000+60*512),58+60,60);
                //while(1);
                asm("mov %esp, 0x7FFFF");
                asm("mov %ebp, %esp");
                ((void (*)())0x100000)();
            }
        }
        for(uint8 i=0;i<9;i++)result[i]=0;

        bus_master=0b00000000;
        ata_identify(bus_master,result);
        if(result[0]==0 && result[7]!=0){
            ata_readByte28bit(bus_master,result2,0,1);
            if(result2[0x1FC]==0xF0&&result2[0x1FD]==0xF0&&result2[0x1FE]==0x55&&result2[0x1FF]==0xAA){
                ata_readByte28bit(bus_master,(uint8*)0x100000,KERNEL_START_SECTOR,KERNEL_SECTOR_COUNT);
                asm("mov %esp, 0x7FFFF");
                asm("mov %ebp, %esp");
                ((void (*)())0x100000)();
            }
        }
        for(uint8 i=0;i<9;i++)result[i]=0;

        bus_master=0b10000001;
        ata_identify(bus_master,result);
        if(result[0]==0 && result[7]!=0){
            ata_readByte28bit(bus_master,result2,0,1);
            if(result2[0x1FC]==0xF0&&result2[0x1FD]==0xF0&&result2[0x1FE]==0x55&&result2[0x1FF]==0xAA){
                ata_readByte28bit(bus_master,(uint8*)0x100000,KERNEL_START_SECTOR,KERNEL_SECTOR_COUNT);
                asm("mov %esp, 0x7FFFF");
                asm("mov %ebp, %esp");
                ((void (*)())0x100000)();
            }
        }
        for(uint8 i=0;i<9;i++)result[i]=0;

        bus_master=0b00000001;
        ata_identify(bus_master,result);
        if(result[0]==0 && result[7]!=0){
            ata_readByte28bit(bus_master,result2,0,1);
            if(result2[0x1FC]==0xF0&&result2[0x1FD]==0xF0&&result2[0x1FE]==0x55&&result2[0x1FF]==0xAA){
                ata_readByte28bit(bus_master,(uint8*)0x100000,KERNEL_START_SECTOR,KERNEL_SECTOR_COUNT);
                asm("mov %esp, 0x7FFFF");
                asm("mov %ebp, %esp");
                ((void (*)())0x100000)();
            }
        }
    #endif
    #if (LOOK_FOR_PCI_STORAGE==1)
    for(uint8 bus=0;bus<=*((uint8*)0x8c2);bus++){
        for(uint8 slot=0;slot<32;slot++){
            if(isMultiFunction(bus,slot)){
                for(uint8 function=0;function<8;function++){
                    if(getVendorNumber(bus,slot,function)!=0xFFFF){
                        checkPciForStorage(bus,slot,function);
                    }
                }
            }else{
                if(getVendorNumber(bus,slot,0)!=0xFFFF){
                    checkPciForStorage(bus,slot,0);
                }
            }
        }
    }
    #endif

    char text[]="No bootable medium found";
    uint32 i=0;
    while(text[i]!=0){
        *(uint16*)(0xB8000+i*2) = text[i] | (0x0f << 8);
        i++;
    }

    while(1);
}

void checkPciForStorage(uint8 bus,uint8 slot,uint8 function){
    if(getClass(bus,slot,function)==0x1 && getSubClass(bus,slot,function)==0x6){ //Device is an AHCI controller
        uint32 abar=getBaseAddress(bus,slot,function,5); // Geting the AHCI base address
        *(uint32*)(abar+0x4) |= 0x80000000; // Set AHCI enable
        *(uint32*)(abar+0x4)=1; // Set HBA reset
        while((*(uint32*)(abar+0x4) & 1)==1); // Wait for HBA reset to clear
        *(uint32*)(abar+0x4) |= 0x80000000; // Set ACHI enable

        uint32 ports_implemented=*((uint32*)(abar+0xc)); // Getting the ports implemented register
        uint8 num_of_ports=(uint8)(*((uint32*)abar) & 0b1111)+1; //Getting the number of ports aviable
        uint8 num_of_drives=0;
        for(uint8 i=0;i<32;i++){ // Looping through the ports
            if((ports_implemented & (1 << i)) != 0){ // Check if the port is implemented
                if((((*(uint32*)(abar+0x100+i*0x80+0x28))) & 0b111) == 0x3){ // Check if a device is detected and communication is established
                    
                }
            }
        }
    }
}

void ata_identify(uint8 bus_master,uint64 result[])
{
    uint8 master=bus_master>>7;
    uint8 bus=bus_master&0x01111111;
    uint16 base_port;
    if(bus==0){base_port=0x1F0;}
    if(bus==1){base_port=0x170;}

    if(inb(base_port+7)==0xFF){
        result[0]=1;
        return;
    }

    outb(base_port+6,master ? 0xB0 : 0xA0);

    inb(base_port+7);
    inb(base_port+7);
    inb(base_port+7);
    inb(base_port+7);
    
    outb(base_port+6,master ? 0xA0 : 0xB0);

    inb(base_port+7);
    inb(base_port+7);
    inb(base_port+7);
    inb(base_port+7);


    if((inb(base_port+7) && 0x40)== 0x40){
        result[0]=2;
        return;
    }

    outb(base_port+2,0);
    outb(base_port+3,0);
    outb(base_port+4,0);
    outb(base_port+5,0);
    outb(base_port+7, 0xEC);

    uint8 cl=inb(base_port + 4);
	uint8 ch=inb(base_port + 5);


    if (cl==0x14 && ch==0xEB) {result[2]=1;}
    else if (cl==0x69 && ch==0x96) {result[2]=2;}
    else if (cl==0 && ch == 0) {result[2]=3;}
    else if (cl==0x3c && ch==0xc3) {result[2]=4;}
    uint8 status = inb(base_port+7);
    if(status==0){
        result[0]=2;
        return;
    }
    uint8 lbalo = inb(base_port+3);
    uint8 lbahi = inb(base_port+5);
    while((status & 0x80 == 0x80) && lbahi==0 && lbalo==0 && (status && 0x1 == 0))
    {
        status = inb(base_port+7);
        lbalo = inb(base_port+3);
        lbahi = inb(base_port+5);
    }
    if(lbahi != 0 || lbalo !=0){
        result[0]=3;
        result[2]=5;
        return;
    }
    if(status && 0x1 == 1){
        uint8 first = inb(base_port+4);
        uint8 second = inb(base_port+5);
        if(first==0x14 && second==0xEB){
            result[0]=3;
            result[2]=5;
            return;
        }else if(first==0x3C && second==0xC3){
            result[0]=3;
            result[2]=4;
            return;
        }
    }
    status = inb(base_port+7);
    while((status & 0x8!=0x8) && (status && 0x1 == 0))
        {
            status = inb(base_port+7);
        }

    if(status && 0x1 == 0){
        uint8 error_code=inb(base_port+1);
        result[0]=4;
        result[1]=error_code;
        return;
    }
    uint16 data_array[256];
    for(uint16 i = 0;i<256;i++){
        data_array[i]=inb16(base_port);
    }
    uint64 bit48_space = (uint64)((uint64)data_array[103] << 48) | (uint64)((uint64)data_array[102] << 32) | (data_array[101] << 16) | data_array[100];
    uint64 bit28_space = ((data_array[61] << 16) | data_array[60]);
    result[3]=data_array[0];
    result[4]=data_array[83];
    result[5]=data_array[88];
    result[6]=data_array[93];
    result[7]=bit28_space;
    result[8]=bit48_space;

    return;
}

void ata_readByte28bit(uint8 bus_master,uint8 result[], uint32 lba, uint8 sector_count){
    uint8 master=bus_master>>7;
    uint8 bus=bus_master&0x01111111;
    uint16 base_port;
    if(bus==0){base_port=0x1F0;}
    if(bus==1){base_port=0x170;}

    outb(base_port+6, master ? 0xA0 : 0xB0);
    inb(base_port+7);
    inb(base_port+7);
    inb(base_port+7);
    inb(base_port+7);

    outb(base_port+6,(uint8)(master ? 0xE0 : 0xF0) | ((lba>>24) & 0x0F));
    outb(base_port+1,0x0);
    outb(base_port+2,sector_count);
    outb(base_port+3,(uint8)lba);
    outb(base_port+4,(uint8)lba>>8);
    outb(base_port+5,(uint8)lba>>16);
    outb(base_port+7,0x20);
    uint8 status;
    uint16 input;
    for(uint16 i = 0; i<sector_count;i++){
        status = inb(base_port+7);
        while((status & 0x8) != 0x8) status = inb(base_port+7);
        for(uint16 j = 0; j<256;j++){
            input=inb16(base_port);
            result[((i*256)+j)*2]=(uint8)input;
            result[((i*256)+j)*2+1]=(uint8)(input>>8);
            
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

uint8 getHeaderType(uint8 bus, uint8 slot){
    return (uint8)(pciRead16(bus,slot,0,14) & 0xFF);
}

uint8 isMultiFunction(uint8 bus, uint8 slot){
    return (getHeaderType(bus,slot) & 0x80) == 0x80;
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
uint16 getDeviceId(uint8 bus, uint8 slot, uint8 function){
    return pciRead16(bus,slot,function,1);
}
uint16 getVendorNumber(uint8 bus, uint8 slot, uint8 function){
    return pciRead16(bus,slot,function,0);
}
uint32 getBaseAddress(uint8 bus,uint8 slot,uint8 function, uint8 barid){
    uint8 header_type=getHeaderType(bus,slot);
    if((header_type & 0b01111111)==0){
        return (pciRead32(bus,slot,function,0x10+barid*4));
    }
    return 0;
}