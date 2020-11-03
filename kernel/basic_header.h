#include"types.h"

#define KEYBOARD_PORT 0x60

uint8 strcmp(char str1[], char str2[]);


void identify(uint8 bus_master,uint64 result[], uint8 log);
void read28bit(uint8 bus_master,uint16 result[], uint32 lba, uint8 sector_count);
void readByte28bit(uint8 bus_master,uint8 result[], uint32 lba, uint8 sector_count);

uint16 getVendorNumber(uint8 bus, uint8 slot);

struct Drive
{
    uint8 type; // 0: Doesn't exist, 1: ATA, 2: AHCI_DRIVE
    uint8 bus_master_slot; // type ATA: msb: masterbit, bit 0 - 6: Bus number | type AHCI_DRIVE: slot number
    uint8 ahci_controller_id; // for AHCI_DRIVEs only
    uint32 sector_count; // Number of sectors
    struct AHCIPort* ahci_port; // for AHCI_DRIVEs only
};

struct PCI_device{
    uint8 bus;
    uint8 slot;
    uint8 function;
};

struct Drive system_drives[20];
uint8 main_drive=0;
uint8 drive_count=0;
uint8 main_drive_bus_master=0b10000000;

struct AHCI_Hardware{
    uint8 implemented;
    struct AHCI_controller* controller;
    struct PCI_device pci_device;
};

struct AHCI_Hardware ahci_controllers[5];

char pciClasses[][35] = {"Unclassified","Mass Storage Controller","Network Controller","Display Controller","Multimedia Controller","Memory Controller","Bridge Device","Simple Communication Controller","Base System Peripheral","Input Device Controller","Docking Station","Processor","Serial Bus Controller","Wireless Controller","Intelligent Controller","Satellite Communication Controller","Encryption Controller","Signal Processing Controller","Processing Accelerator","Non-Essential Instrumentation"};