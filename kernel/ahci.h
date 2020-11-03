#include "kernel.h"

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

typedef volatile struct AHCI_controller{
    struct AHCI_generic_host_control generic_host_control;
    uint8 resv[116];
    uint8 vendor_specific[96];
    struct AHCIPort ports[];
};

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

void mapAHCI(uint8 log){
    for(uint8 i = 0;i<5;i++){
        if(ahci_controllers[i].implemented){

            ahci_controllers[i].controller->generic_host_control.global_host_control |= 0x1;
            while((ahci_controllers[i].controller->generic_host_control.global_host_control & 0x1) != 0);
            ahci_controllers[i].controller->generic_host_control.global_host_control |= 0x80000000;

            uint8 implp = 0;
            uint8 driveCount = 0;
            for(uint8 j= 0;j<32;j++){
                if((ahci_controllers[i].controller->generic_host_control.ports_implemented & (0x1 << j)) != 0){
                    implp++;
                    if((ahci_controllers[i].controller->ports[j].SATA_status & 0x1) != 0 && (ahci_controllers[i].controller->ports[j].SATA_status & 0x1) != 4){
                        driveCount++;
                        system_drives[drive_count].type=2;
                        system_drives[drive_count].ahci_controller_id=i;
                        system_drives[drive_count].bus_master_slot=j;
                        system_drives[drive_count].ahci_port=&ahci_controllers[i].controller->ports[j];
                        drive_count++;



                    }
                }
            }
            if(log){
                print("AHCI controller #");
                printint(i);
                print(":");
                nextLine();
                print(" Number of ports: ");
                printint(implp);
                nextLine();
                print(" Number of drives: ");
                printint(driveCount);
                nextLine();
                print(" Version: ");
                printint(ahci_controllers[i].controller->generic_host_control.version >> 16);
                print(".");
                printint((uint16)ahci_controllers[i].controller->generic_host_control.version);
            }
        }
    }
}