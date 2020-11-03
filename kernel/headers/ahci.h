#ifndef _AHCI_H
#define _AHCI_H
#include "types.h"
#include "pci.h"

typedef volatile struct{
    uint64 data_base_address;
    uint32 resv;
    uint32 dw3;
} AHCI_physical_region;

typedef volatile struct{
    AHCI_physical_region physical_regions[5];
} AHCI_physical_region_descriptor_table;

typedef volatile struct{
    uint8 bytes[64];
} AHCI_command_fis;

typedef volatile struct{
    uint8 bytes[16];
} ATAPI_command;

typedef volatile struct{
    AHCI_command_fis command_fis;
    ATAPI_command atapi_command;
    uint8 resv[0x30];
    AHCI_physical_region_descriptor_table physical_region_descriptor_table;
} AHCI_command_table;

typedef volatile struct{
    uint8 command_fis_length:5;
    uint8 ATAPI:1;
    uint8 write:1;
    uint8 prefetchable:1;
    uint8 reset:1;
    uint8 bist:1;
    uint8 clear_busy_upon_R_OK:1;
    uint8 resv0:1;
    uint8 port_multiplier_port:4;
    uint16 physical_region_descriptor_table_length;
    uint32 physical_region_descriptor_byte_count;
    uint32 command_descriptor_table_base_address;
    uint32 command_table_descriptor_base_address_upper;
    uint32 resv[4];
} AHCI_command;

typedef volatile struct{
    AHCI_command commands[32];
} AHCI_command_list;

typedef volatile struct{
    AHCI_command_list *command_list;
    uint32 command_list_base_address_upper;
    uint32 FIS_base_address;
    uint32 FIS_base_address_upper;
    uint32 interrupt_status;
    uint32 interrupt_enable;
    uint32 command_status;
    uint32 resv;
    uint32 task_file_data;
    uint32 signature;
    uint32 SATA_status;
    uint32 SATA_control;
    uint32 SATA_error;
    uint32 SATA_active;
    uint32 command_issue;
    uint32 SATA_notification;
    uint32 FIS_based_switching_control;
    uint32 device_sleeep;
    uint32 resv2[10];
    uint32 vendor_specific[4];
} AHCIPort;

typedef volatile struct{
    uint32 host_capabilities;
    uint32 global_host_control;
    uint32 interrupt_status;
    uint32 ports_implemented;
    uint32 version;
    uint32 command_completion_coalescing_control;
    uint32 command_completion_coalescing_ports;
    uint32 enclousre_management_location;
    uint32 enclousre_management_control;
    uint32 host_capabilities_extended;
    uint32 BIOS_OS_handoff_control_and_status;
} AHCI_generic_host_control;

typedef volatile struct{
    AHCI_generic_host_control generic_host_control;
    uint8 resv[116];
    uint8 vendor_specific[96];
    AHCIPort ports[];
} AHCI_controller;

typedef enum{
    AHCI_FIS_reg_h2d =   0x27,
    AHCI_FIS_reg_d2h =   0x34,
    AHCI_FIS_dma_act =   0x39,
    AHCI_FIS_dma_setup = 0x41,
    AHCI_FIS_data =      0x46,
    AHCI_FIS_bist =      0x58,
    AHCI_FIS_pio_setup = 0x5f,
    AHCI_FIS_dev_bits =  0xa1
} fis_type;

struct AHCI_Hardware{
    uint8 implemented;
    AHCI_controller* controller;
    uint8 ports_implemented;
    uint8 drive_count;
    PCI_device pci_device;
};

typedef struct tagFIS_REG_H2D
{
	// DWORD 0
	uint8  fis_type;	// FIS_TYPE_REG_H2D
 
	uint8  pmport:4;	// Port multiplier
	uint8  rsv0:3;		// Reserved
	uint8  c:1;		// 1: Command, 0: Control
 
	uint8  command;	// Command register
	uint8  featurel;	// Feature register, 7:0
 
	// DWORD 1
	uint8  lba0;		// LBA low register, 7:0
	uint8  lba1;		// LBA mid register, 15:8
	uint8  lba2;		// LBA high register, 23:16
	uint8  device;		// Device register
 
	// DWORD 2
	uint8  lba3;		// LBA register, 31:24
	uint8  lba4;		// LBA register, 39:32
	uint8  lba5;		// LBA register, 47:40
	uint8  featureh;	// Feature register, 15:8
 
	// DWORD 3
	uint8  countl;		// Count register, 7:0
	uint8  counth;		// Count register, 15:8
	uint8  icc;		// Isochronous command completion
	uint8  control;	// Control register
 
	// DWORD 4
	uint8  rsv1[4];	// Reserved
} AHCI_FIS_h2d;

struct AHCI_Hardware ahci_controllers[5];

void setUpAHCI(uint32 deviceID);
void mapAHCI();

#endif