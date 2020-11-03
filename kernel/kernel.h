#ifndef _KERNEL_H_
#define _KERNEL_H_
#include"types.h"

#define VGA_ADDRESS 0xB8000
#define KEYBOARD_PORT 0x60
#define WIDTH 80
#define HEIGHT 25

#define BLACK 0x0
#define DARK_BLUE 0x1
#define DARK_GREEN 0x2
#define DARK_AQUA 0x3
#define DARK_RED 0x4
#define PURPLE 0x5
#define DARK_YELLOW 0x6
#define LIGHT_GRAY 0x7
#define DARK_GRAY 0x8
#define LIGHT_BLUE 0x9
#define LIGHT_GREEN 0xa
#define LIGHT_AQUA 0xb
#define LIGHT_RED 0xc
#define MAGENTA 0xd
#define LIGHT_YELLOW 0xe
#define WHITE 0xf

char ascii_keymap[128];
char shift_ascii_keymap[128];


uint16* TERMINAL_BUFFER;
uint8 forecolor = WHITE;
uint8 backcolor =BLACK;
uint16 vga_index = 0;

void outb(uint16 port, uint8 data);
uint8 inb(uint16 port);
void goxy(uint8 x, uint8 y);
void print_int(uint64 number, uint8 fore_color, uint8 back_color);
void printint(uint32 number);
void printhex(uint32 number);
void print(char text[]);
void nextLine();
void write_letter(char letter, uint8 fore_color, uint8 back_color);
void outb32(uint16 port, uint32 data);
uint32 inb32(uint16 port);
uint8 strcmp(char str1[], char str2[]);
void printFloat(double n);
void switchMode(uint8 mode);
void drawTriangle(uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 x3, uint16 y3, uint8 color);
void drawLine(uint16 x1,uint16 y1,uint16 x2,uint16 y2,uint8 color);
void drawRectangle(uint16 x, uint16 y, uint16 w, uint16 h, uint8 color);
void drawRectangleFilled(uint16 x, uint16 y, uint16 w, uint16 h, uint8 color);
void put_pixel(uint16 x, uint16 y, uint8 color);
void video_clear();
double root(double n);

void identify(uint8 bus_master,uint64 result[], uint8 log);
void read28bit(uint8 bus_master,uint16 result[], uint32 lba, uint8 sector_count);
void readByte28bit(uint8 bus_master,uint8 result[], uint32 lba, uint8 sector_count);

uint16 getVendorNumber(uint8 bus, uint8 slot);
uint8 next_keyboard_byte();
uint8 addKeyboardEvent(void (*handler)(uint8));
//uint32 getBaseAddress(struct PCI_device* pci_device, uint8 barid);

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

uint64* memroy_map_region_base;
uint64* memory_map_region_length;
uint8* memory_map_region_type;
uint8 memory_map_region_count;

struct AHCI_Hardware{
    uint8 implemented;
    struct AHCI_controller* controller;
    struct PCI_device pci_device;
};

struct AHCI_Hardware ahci_controllers[5];



char hexLetters[]="0123456789ABCDEF";

char pciClasses[][35] = {"Unclassified","Mass Storage Controller","Network Controller","Display Controller","Multimedia Controller","Memory Controller","Bridge Device","Simple Communication Controller","Base System Peripheral","Input Device Controller","Docking Station","Processor","Serial Bus Controller","Wireless Controller","Intelligent Controller","Satellite Communication Controller","Encryption Controller","Signal Processing Controller","Processing Accelerator","Non-Essential Instrumentation"};



//uint8 inb(uint16);

#endif
