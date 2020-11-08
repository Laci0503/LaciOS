#include "basic_header.h"
#include "drives.h"
#include "disk_driver.h"
#include "pci.h"
#include "ahci.h"
#include "libs.h"

void load_kernel(){
    map_ata(0);
    mapPci(0);
    mapAHCI(0);
    readFromDriveByte(system_drives[main_drive],(uint8*)0x100000,10,120);
}