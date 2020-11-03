#include"kernel.h"

/*
    0: Error code: 0: No error, 1: Floating bus, 2: No drive, 3: Not readable ATA type, 4: Unexpected error
    1: Error code, if exists
    2: Drive type: 0: Unknown, 1: PATAPI, 2: SATAPI, 3: PATA, 4: SATA, 5: ATAPI
    3: uint16_t 0: is useful if the device is not a hard disk. 
    4: uint16_t 83: Bit 10 is set if the drive supports LBA48 mode. 
    5: uint16_t 88: The bits in the low byte tell you the supported UDMA modes, the upper byte tells you which UDMA mode is active. If the active mode is not the highest supported mode, you may want to figure out why. 
    6: uint16_t 93 from a master drive on the bus: Bit 11 is supposed to be set if the drive detects an 80 conductor cable. Notes: if the bit is set then 80 conductor cable is present and UDMA modes > 2 can be used; if bit is clear then there may or may not be an 80 conductor cable and UDMA modes > 2 shouldn't be used but might work fine. Because this bit is "master device only", if there is a slave device and no master there is no way information about cable type (and would have to assume UDMA modes > 2 can't be used). 
    7: uint16_t 60 & 61 taken as a uint32_t contain the total number of 28 bit LBA addressable sectors on the drive. (If non-zero, the drive supports LBA28.) 
    8: uint16_t 100 through 103 taken as a uint64_t contain the total number of 48 bit addressable sectors on the drive. (Probably also proof that LBA48 is supported.) 
*/
void identify(uint8 bus_master,uint64 result[], uint8 log)
{
    uint8 master=bus_master>>7;
    uint8 bus=bus_master&0x01111111;
    uint16 base_port;
    if(bus==0){base_port=0x1F0;}
    if(bus==1){base_port=0x170;}

    if(inb(base_port+7)==0xFF){
        if(log)print("Floating bus");
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
        print("No drive");
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


    if (cl==0x14 && ch==0xEB) {if(log)print("PATAPI drive"); result[2]=1;}
    else if (cl==0x69 && ch==0x96) {if(log)print("SATAPI drive");result[2]=2;}
    else if (cl==0 && ch == 0) {if(log)print("PATA drive");result[2]=3;}
    else if (cl==0x3c && ch==0xc3) {if(log)print("SATA drive");result[2]=4;}
    else print("UNKNOWN drive");
    if(log)nextLine();
    uint8 status = inb(base_port+7);
    if(status==0){
        if(log){print("Drive does not exist");}
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
        if(log){print("Probably ATAPI.");}
        result[0]=3;
        result[2]=5;
        return;
    }
    if(status && 0x1 == 1){
        uint8 first = inb(base_port+4);
        uint8 second = inb(base_port+5);
        if(first==0x14 && second==0xEB){
            if(log)print(" ATAPI drive");
            result[0]=3;
            result[2]=5;
            return;
        }else if(first==0x3C && second==0xC3){
            if(log)print(" SATA drive"); 
            result[0]=3;
            result[2]=4;
            return;
        }
        //nextLine();
        /*print("Error: ");
        print_int(inb(base_port+1),forecolor, backcolor);*/
        //return;
        //nextLine();
    }
    status = inb(base_port+7);
    while((status & 0x8!=0x8) && (status && 0x1 == 0))
        {
            status = inb(base_port+7);
        }

    if(status && 0x1 == 0){
        uint8 error_code=inb(base_port+1);
        if(log){
            print("Error: ");
            print_int(error_code,forecolor, backcolor);
        }
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
    if(log){
        nextLine();
        print("Storage space: 28-bit mode: ");
        print_int(bit28_space * 512,forecolor,backcolor);
        print(" bytes");
        print(" / 48-bit mode: ");
        print_int(bit48_space * 512,forecolor,backcolor);
        print(" bytes");
    }
    result[3]=data_array[0];
    result[4]=data_array[83];
    result[5]=data_array[88];
    result[6]=data_array[93];
    result[7]=bit28_space;
    result[8]=bit48_space;

    return;
}

void read28bit(uint8 bus_master,uint16 result[], uint32 lba, uint8 sector_count){
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
    for(uint16 i = 0; i<sector_count;i++){
        status = inb(base_port+7);
        while((status & 0x8) != 0x8) status = inb(base_port+7);
        for(uint16 j = 0; j<256;j++){
            result[(i*256)+j]=inb16(base_port);
        }
    }
    return;
}

void readByte28bit(uint8 bus_master,uint8 result[], uint32 lba, uint8 sector_count){
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