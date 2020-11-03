#include "../headers/ahci.h"
#include "../headers/drives.h"

void mapAHCI(){
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
                    if((ahci_controllers[i].controller->ports[j].SATA_status & 0x1) != 0){
                        driveCount++;
                        system_drives[drive_count].type=2;
                        system_drives[drive_count].ahci_controller_id=i;
                        system_drives[drive_count].bus_master_slot=j;
                        system_drives[drive_count].ahci_port=&ahci_controllers[i].controller->ports[j];
                        drive_count++;
                        
                        AHCI_command_table command_desc_table;
                        for(uint16 i=0;i<sizeof(AHCI_command_table);i++)*(uint8*)(&command_desc_table+i)=0;

                        AHCI_FIS_h2d *id_fis=(AHCI_FIS_h2d*)&command_desc_table.command_fis;
                        //for(uint16 i=0;i<sizeof(AHCI_FIS_h2d);i++)*(uint8*)(&id_fis+i)=0;

                        id_fis->fis_type=AHCI_FIS_reg_h2d;
                        id_fis->c=1;
                        id_fis->command=0xEC;

                        ahci_controllers[i].controller->ports[j].command_list->commands[0].ATAPI=0;
                        ahci_controllers[i].controller->ports[j].command_list->commands[0].clear_busy_upon_R_OK=1;
                        ahci_controllers[i].controller->ports[j].command_list->commands[0].command_fis_length=sizeof(AHCI_FIS_h2d);
                        ahci_controllers[i].controller->ports[j].command_list->commands[0].port_multiplier_port=0;
                        ahci_controllers[i].controller->ports[j].command_list->commands[0].prefetchable=1;
                        ahci_controllers[i].controller->ports[j].command_list->commands[0].bist=0;
                        ahci_controllers[i].controller->ports[j].command_list->commands[0].reset=0;
                        ahci_controllers[i].controller->ports[j].command_list->commands[0].resv0=0;
                        ahci_controllers[i].controller->ports[j].command_list->commands[0].resv[0]=0;
                        ahci_controllers[i].controller->ports[j].command_list->commands[0].resv[1]=0;
                        ahci_controllers[i].controller->ports[j].command_list->commands[0].resv[2]=0;
                        ahci_controllers[i].controller->ports[j].command_list->commands[0].resv[3]=0;
                        ahci_controllers[i].controller->ports[j].command_list->commands[0].write=0;
                        ahci_controllers[i].controller->ports[j].command_list->commands[0].command_table_descriptor_base_address_upper=0;
                        ahci_controllers[i].controller->ports[j].command_list->commands[0].physical_region_descriptor_table_length=0;
                        ahci_controllers[i].controller->ports[j].command_list->commands[0].physical_region_descriptor_byte_count=0;
                        ahci_controllers[i].controller->ports[j].command_list->commands[0].command_descriptor_table_base_address=(uint32)&command_desc_table;
                        uint16 data[256+2];
                        for(uint16 i=0;i<sizeof(data);i++)data[i]=0;
                        ahci_controllers[i].controller->ports[j].FIS_base_address=(uint32)&data;

                        ahci_controllers[i].controller->ports[j].command_issue|=0x1;
                        ahci_controllers[i].controller->ports[j].command_status|=0x1;
                    }
                }
            }
            ahci_controllers[i].ports_implemented=implp;
            ahci_controllers[i].drive_count=driveCount;
        }
    }
}

void setUpAHCI(uint32 deviceID){
    
}