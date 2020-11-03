#include "../headers/drives.h"
#include "../headers/ata.h"
#include "../headers/video.h"
#include "../headers/string.h"

void map_ata(){
    uint64 result[9];
    uint16 result2[256];
    for(uint8 i=0;i<9;i++)result[i]=0;

    //Primary, master
    identify(0b10000000,result);
    if(result[0]==0 && result[7]!=0){
        system_drives[drive_count].type=1;
        system_drives[drive_count].bus_master_slot=0b10000000;
        system_drives[drive_count].sector_count=result[7];
        read28bit(0b10000000,result2,0,1);
        if(result2[255]==0xaa55 && result2[254] == 0xF0F0){
            main_drive=drive_count;
        }
        drive_count++;
    }

    //Primary slave
    for(uint8 i=0;i<9;i++)result[i]=0;
    identify(0b00000000,result);
    if(result[0]==0 && result[7]!=0){
        system_drives[drive_count].type=1;
        system_drives[drive_count].bus_master_slot=0b00000000;
        system_drives[drive_count].sector_count=result[7];
        read28bit(0b00000000,result2,0,1);
        if(result2[255]==0xaa55 && result2[254] == 0xF0F0){
            main_drive=drive_count;
        }
        drive_count++;
    }

    //Secondary master
    for(uint8 i=0;i<9;i++)result[i]=0;
    identify(0b10000001,result);
    if(result[0]==0 && result[7]!=0){

        system_drives[drive_count].type=1;
        system_drives[drive_count].bus_master_slot=0b10000001;
        system_drives[drive_count].sector_count=result[7];

        read28bit(0b10000001,result2,0,1);
        if(result2[255]==0xaa55 && result2[254] == 0xF0F0){
            main_drive=drive_count;
        }
        drive_count++;
    }

    //Secondary slave
    for(uint8 i=0;i<9;i++)result[i]=0;
    identify(0b00000001,result);
    if(result[0]==0 && result[7]!=0){

        system_drives[drive_count].type=1;
        system_drives[drive_count].bus_master_slot=0b00000001;
        system_drives[drive_count].sector_count=result[7];

        read28bit(0b00000001,result2,0,1);
        if(result2[255]==0xaa55 && result2[254] == 0xF0F0){
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
void driveInit(){
    main_drive=0;
    drive_count=0;
}
void exportDrives(uint8* address){
    uint8* pointer=(uint8*) system_drives;
    for(uint16 i=0;i<sizeof(struct Drive)*20;i++){
        address[i]=pointer[i];
    }
    pointer+=sizeof(struct Drive)*20;
    *pointer=main_drive;
    pointer++;
    *pointer=drive_count;
}
void importDrives(uint8* address){
    uint8* pointer=(uint8*) system_drives;
    for(uint16 i=0;i<sizeof(struct Drive)*20;i++){
        pointer[i]=address[i];
    }
    main_drive=*(address+sizeof(struct Drive)*20);
    drive_count=*(address+sizeof(struct Drive)*20+1);
}