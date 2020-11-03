#include "kernel.h"

void map_ata(uint8 printlog){
    uint64 result[9];
    uint16 result2[256];
    for(uint8 i=0;i<9;i++)result[i]=0;

    //Primary, master
    identify(0b10000000,result, 0);
    if(result[0]==0 && result[7]!=0){
        if(printlog){
            print("Primary/Master: Disk detected, space: ");
            print_int(result[7],forecolor, backcolor);
            print(" sectors, type: ");
            if(result[2]==0) print("Unknown");
            else if(result[2]==1) print("PATAPI");
            else if(result[2]==2) print("SATAPI");
            else if(result[2]==3) print("PATA");
            else if(result[2]==4) print("SATA");
            else if(result[2]==4) print("ATAPI");
        }
        system_drives[drive_count].type=1;
        system_drives[drive_count].bus_master_slot=0b10000000;
        system_drives[drive_count].sector_count=result[7];
        read28bit(0b10000000,result2,0,1);
        if(result2[255]==0xaa55 && result2[254] == 0xF0F0){
            if(printlog){
                nextLine();
                print("Boot drive: Primary bus, master drive");
            }
            main_drive=drive_count;
        }
        drive_count++;
    }

    //Primary slave
    for(uint8 i=0;i<9;i++)result[i]=0;
    identify(0b00000000,result, 0);
    if(result[0]==0 && result[7]!=0){
        if(printlog){
            nextLine();
            print("Primary/Slave Disk detected, space: ");
            print_int(result[7],forecolor, backcolor);
            print(" sectors, type: ");
            if(result[2]==0) print("Unknown");
            else if(result[2]==1) print("PATAPI");
            else if(result[2]==2) print("SATAPI");
            else if(result[2]==3) print("PATA");
            else if(result[2]==4) print("SATA");
            else if(result[2]==4) print("ATAPI");
        }
        system_drives[drive_count].type=1;
        system_drives[drive_count].bus_master_slot=0b00000000;
        system_drives[drive_count].sector_count=result[7];
        read28bit(0b00000000,result2,0,1);
        if(result2[255]==0xaa55 && result2[254] == 0xF0F0){
            if(printlog){
                nextLine();
                print("Boot drive: Primary bus, slave drive");
            }
            main_drive=drive_count;
        }
        drive_count++;
    }

    //Secondary master
    for(uint8 i=0;i<9;i++)result[i]=0;
    identify(0b10000001,result, 0);
    if(result[0]==0 && result[7]!=0){
        if(printlog){
            nextLine();
            print("Secondary/Master Disk detected, space: ");
            print_int(result[7],forecolor, backcolor);
            print(" sectors, type: ");
            if(result[2]==0) print("Unknown");
            else if(result[2]==1) print("PATAPI");
            else if(result[2]==2) print("SATAPI");
            else if(result[2]==3) print("PATA");
            else if(result[2]==4) print("SATA");
            else if(result[2]==4) print("ATAPI");
        }

        system_drives[drive_count].type=1;
        system_drives[drive_count].bus_master_slot=0b10000001;
        system_drives[drive_count].sector_count=result[7];

        read28bit(0b10000001,result2,0,1);
        if(result2[255]==0xaa55 && result2[254] == 0xF0F0){
            if(printlog){
                nextLine();
                print("Boot drive: Secondary bus, master drive");
            }
            main_drive=drive_count;
        }
        drive_count++;
    }

    //Secondary slave
    for(uint8 i=0;i<9;i++)result[i]=0;
    identify(0b00000001,result, 0);
    if(result[0]==0 && result[7]!=0){
        if(printlog){
            nextLine();
            print("Secondary/Slave Disk detected, space: ");
            print_int(result[7],forecolor, backcolor);
            print(" sectors, type: ");
            if(result[2]==0) print("Unknown");
            else if(result[2]==1) print("PATAPI");
            else if(result[2]==2) print("SATAPI");
            else if(result[2]==3) print("PATA");
            else if(result[2]==4) print("SATA");
            else if(result[2]==4) print("ATAPI");
        }

        system_drives[drive_count].type=1;
        system_drives[drive_count].bus_master_slot=0b00000001;
        system_drives[drive_count].sector_count=result[7];

        read28bit(0b00000001,result2,0,1);
        if(result2[255]==0xaa55 && result2[254] == 0xF0F0){
            if(printlog){
                nextLine();
                print("Boot drive: Secondary bus, slave drive");
            }
            main_drive=drive_count;
        }
        drive_count++;
    }
}

void readFromDrive(struct Drive drive, uint16 result[], uint32 lba, uint8 sector_count){
    if(drive.type==1){
        read28bit(drive.bus_master_slot,result,lba,sector_count);
        return;
    }
}
void readFromDriveByte(struct Drive drive, uint8 result[], uint32 lba, uint8 sector_count){
    if(drive.type==1){
        readByte28bit(drive.bus_master_slot,result,lba,sector_count);
        return;
    }
}