#ifndef _PCI_H
#define _PCI_H
#include "types.h"

#define pci_company_start_sector 131
#define pci_company_length 20923
#define pci_company_length_sectors 41
#define pci_company_entry_count 862
#define pci_company_name_length 73

#define pci_m1_address_port 0xCF8
#define pci_m1_data_port 0xCFC

char pci_company_names[pci_company_entry_count][pci_company_name_length];
uint16 pci_company_values[pci_company_entry_count];

typedef struct{
    uint8 bus;
    uint8 slot;
    uint8 function;
    uint8 class;
    uint8 subclass;
    uint16 vendorID;
    uint16 deviceId;
    uint8 progIf;
    uint8 multifunction;
    uint8 headerType;
} PCI_device;

PCI_device pciDevices[20];
uint16 pciDeviceCount;

char pciSubClassNames[13][30];

char pciClasses[20][35];

void load_pci_companies(char names[][pci_company_name_length], uint16 values[]);
uint16 pciRead16(uint8 bus, uint8 slot, uint8 function, uint8 offset);
uint32 pciRead32(uint8 bus, uint8 slot, uint8 function, uint8 offset);
void pciWrite16(uint8 bus, uint8 slot, uint8 function, uint8 offset, uint16 data);
void pciWrite32(uint8 bus, uint8 slot, uint8 function, uint8 offset, uint32 data);
uint16 getVendorNumber(uint8 bus, uint8 slot);
uint8 getHeaderType(uint8 bus, uint8 slot);
uint8 isMultiFunction(uint8 bus, uint8 slot);
uint8 getClass(uint8 bus, uint8 slot, uint8 function);
uint8 getSubClass(uint8 bus, uint8 slot, uint8 function);
uint16 getProgIF(uint8 bus, uint8 slot, uint8 function);
uint8 getSubClassName(uint8 class, uint8 subClass);
uint32 getBaseAddress(PCI_device* pci_device, uint8 barid);
void mapPci();
void printPciInfo();
void pciInit();
void savePciLayout(uint8* address);
void loadPciLayout(uint8* address);

#endif