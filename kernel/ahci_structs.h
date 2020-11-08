#include "types.h"
// AHCI
typedef volatile struct AHCI_physical_region{
    uint64 data_base_address;
    uint32 resv;
    uint32 dw3;
};

typedef volatile struct AHCI_physical_region_descriptor_table{
    struct AHCI_physical_region* physical_regions;
};

typedef volatile struct AHCI_command_fis{
    uint8 bytes[64];
};

typedef volatile struct ATAPI_command{
    uint8 bytes[16];
};

typedef volatile struct AHCI_command_table{
    struct AHCI_command_fis command_fis;
    struct ATAPI_command atapi_command;
};

typedef volatile struct AHCI_command{
    uint16 firt_word;
    uint16 physical_region_descriptor_table_length;
    uint32 physical_region_descriptor_byte_count;
    uint32 command_descriptor_table_base_address;
    uint32 command_table_descriptor_base_address_upper;
    uint32 resv[4];
};

typedef volatile struct AHCI_command_list{
    struct AHCI_command commands[32];
};

typedef volatile struct AHCIPort{
    struct AHCI_command_list* command_list;
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
};

typedef volatile struct AHCI_generic_host_control{
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
};

typedef volatile struct{
    struct AHCI_generic_host_control generic_host_control;
    uint8 resv[116];
    uint8 vendor_specific[96];
    struct AHCIPort ports[];
} AHCI_controller;

struct AHCI_Hardware{
    uint8 implemented;
    AHCI_controller* controller;
    struct PCI_device pci_device;
};

struct AHCI_Hardware ahci_controllers[5];