#ifndef _ATA_H
#define _ATA_H
#include "types.h"

/*
    0: Error code: 0: No error, 1: Floating bus, 2: No drive, 3: Not readable ATA type, 4: Unexpected error
    1: Error code, if exists
    2: Drive type: 0: Unknown, 1: PATAPI, 2: SATAPI, 3: PATA, 4: SATA, 5: ATAPI
    3: uint16_t 0: is useful if the device is not a hard disk. 
    4: uint16_t 83: Bit 10 is set if the drive supports LBA48 mode. 
    5: uint16_t 88: The bits in the low byte tell you the supported UDMA modes, the upper byte tells you which UDMA mode is active. If the active mode is not the highest supported mode, you may want to figure out why. 
    6: uint16_t 93 from a master drive on the bus: Bit 11 is supposed to be set if the drive detects an 80 conductor cable. Notes: if the bit is set then 80 conductor cable is present and UDMA modes > 2 can be used; if bit is clear then there may or may not be an 80 conductor cable and UDMA modes > 2 shouldn't be used but might work fine. Because this bit is "master device only", if there is a slave device and no master there is no way information about cable type (and would have to assume UDMA modes > 2 can't be used). 
    7: uint16_t 60 & 61 taken as a uint32_t contain the total number of 28 bit LBA addressable sectors on the drive. (If non-zero, the drive supports LBA28.) 
    8: uint16_t 100 through 103 taken as a uint64_t contain the total number of 48 bit addressable sectors on the drive. (Probably also proof that LBA48 is supported.) 
*/
void identify(uint8 bus_master,uint64 result[]);
void read28bit(uint8 bus_master,uint16 result[], uint32 lba, uint8 sector_count);
void readByte28bit(uint8 bus_master,uint8 result[], uint32 lba, uint8 sector_count);

#endif