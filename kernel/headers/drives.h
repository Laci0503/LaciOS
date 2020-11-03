#ifndef _DRIVES_H
#define _DRIVES_H
#include "types.h"
#include "ahci.h"

typedef enum{
    DRIVE_BUS_TYPE_NONE,
    DRIVE_BUS_TYPE_ATA,
    DRIVE_BUS_TYPE_AHCI,
    DRIVE_BUS_TYPE_UNKNOWN =   0xFE,
    DRIVE_BUS_TYPE_OTHER   =   0xFF

} DRIVE_BUS_TYPE;

struct Drive
{
    DRIVE_BUS_TYPE type; // 0: Doesn't exist, 1: ATA, 2: AHCI_DRIVE
    uint8 bus_master_slot; // type ATA: msb(but 7): masterbit, bit 0 - 6: Bus number | type AHCI_DRIVE: slot number
    uint8 ahci_controller_id; // for AHCI_DRIVEs only
    uint32 sector_count; // Number of sectors
    AHCIPort* ahci_port; // for AHCI_DRIVEs only
};

struct Drive system_drives[20];
uint8 main_drive;
uint8 drive_count;

void map_ata();
void readFromDrive(struct Drive drive, uint16 result[], uint32 lba, uint8 sector_count);
void readFromDriveByte(struct Drive drive, uint8 result[], uint32 lba, uint8 sector_count);
void driveInit();
void exportDrives(uint8* address);
void importDrives(uint8* address);


#endif