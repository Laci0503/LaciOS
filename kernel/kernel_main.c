#include "kernel.h"
#include "libs.h"
#include "disk_driver.h"
#include "pci.h"
#include "memory_map.h"
#include "drives.h"
#include "ahci.h"
#include "interrupts.h"
#include "video.h"
#include "math.h"
#include "fun.h"

#define print_pci_info 0
#define print_memory_info 0
#define print_ata_log 0
#define print_ahci_log 0

void test2(uint8 keycode){
    return;
}

void terminal_keydown(uint8 keycode);
char command_buffer[80];
uint16 cmd_pos=0;
uint8 lastFinished=1;

void _KERNEL_ENTRY(){
    TERMINAL_BUFFER=(uint16*) VGA_ADDRESS;
    clear();
    init_keymap();
    goxy(0,0);
    //print("Testing system");

    nextLine();
    for(uint8 i = 0;i<20;i++)system_drives[i].type=0;
    map_ata(print_ata_log);
    load_memory_map();
    memory_init();
    if(print_memory_info){
        nextLine();
        nextLine();
        print_memory_map();
    }
    load_pci_companies(pci_company_names,pci_company_values);
    if(print_pci_info){
        printPciInfo();
        nextLine();
    }
    mapPci(print_pci_info);
    mapAHCI(print_ahci_log);

    setupInterrupts();

    char command[80];

    /*struct cmx a;
    a.imaginaryPart=5;
    a.realPart=3;
    struct cmx b;
    b.realPart=4;
    b.imaginaryPart=7;*/

    //struct cmx* c=sq(&a);

    /*printsignedint(c->realPart);
    print("   ");
    printsignedint(c->imaginaryPart);*/


    /*while(1){
        clear();
        goxy(0,0);
        forecolor=LIGHT_GREEN;
        print("Type a command!");
        read(command,0);
        if(strcmp(command, "mandelbrot")){
            drawMandelbort();
        }
        for(uint8 i=0; i<80; i++) command[i]=0;
    }*/

    lastFinished=1;

    //goxy(0,24);
    addKeyboardEvent(terminal_keydown);
    //addKeyboardEvent(test2);
    uint8 state=0;
    uint8 test;
    while(0){
        if(lastFinished){
            if(state){
                goxy(cmd_pos,24);
                print("_");
            }else{
                goxy(cmd_pos,24);
                print(" ");
            }
            state=!state;
            //sleep(50000,2000);
        }
    }

    /*switchMode(0x13);
    drawRectangle(0,0,320,200,0x4);*/


    while(1){};
}

void sleep(uint32 n, uint32 m){
    for(uint32 i=0;i<n;i++){
        for(uint32 j=0;j<m;j++){
            asm("NOP");
        }
    }
}

void terminal_keydown(uint8 keycode){
    if(lastFinished){
        uint8 code=keycode;
        uint8 pushed = (keycode & 0x80) == 0;
        if(pushed){
            if(ascii_keymap[code]!=0){
                if(cmd_pos<WIDTH-2){
                    cmd_pos++;
                    command_buffer[cmd_pos-1]=ascii_keymap[code];
                    goxy(cmd_pos-1,24);
                    write_letter(command_buffer[cmd_pos-1],forecolor,backcolor);
                }
            }else{
                if(code==14){
                    if(cmd_pos!=0){
                        cmd_pos--;
                        goxy(cmd_pos,24);
                        print("  ");
                        command_buffer[cmd_pos]=0;
                    }
                }else if(code==28){
                    if(strcmp(command_buffer, "mandelbrot")){
                        lastFinished=0;
                        drawMandelbrot();
                    }
                    goxy(cmd_pos,24);
                    print(" ");
                    nextLine();
                    cmd_pos=0;
                    for(uint8 i=0; i<80;i++)command_buffer[i]=0;
                    lastFinished=1;
                }
            }
        }
    }
}