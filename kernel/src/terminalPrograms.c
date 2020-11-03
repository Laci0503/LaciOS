#include "../headers/terminalPrograms.h"
#include "../headers/types.h"
#include "../headers/ata.h"
#include "../headers/drives.h"
#include "../headers/input.h"
#include "../headers/interrupts.h"
#include "../headers/string.h"
#include "../headers/video.h"
#include "../headers/pci.h"
#include "../headers/memory.h"
#include "../headers/advanced_string.h"
#include "../headers/mandelbrot.h"
#include "../headers/infoPrinters.h"
#include "../headers/deviceManager.h"

void init_terminal_program_list(){
    char names[][50]={
        "test",
        "test2",
        "mmap",
        "memmanager",
        "pci",
        "mandelbrot",

    };

    uint8* funcitons[]={
        (uint8*)&terminal_program_test,
        (uint8*)&terminal_program_test2,
        (uint8*)&mmap_terminal_command,
        (uint8*)&memory_manager_terminal_command,
        (uint8*)&pci_terminal_command,
        (uint8*)&MandelbrotConsoleCommand

    };
    program_count=sizeof(names)/50;
    for(uint16 i=0;i<program_count;i++){
        Terminal_programs[i].function_pointer=(void (*)(string_array *))funcitons[i];
        uint8 l=len(names[i]);
        Terminal_programs[i].name=malloc(l);
        memcpy(names[i],Terminal_programs[i].name,l);
        uint32 cs=0;
        uint8 index=0;
        while(names[i][index]!=0){
            cs+=names[i][index];
            index++;
        }
        Terminal_programs[i].name_checksum=cs;
    }
}

void terminal_program_test(string_array* args){
    //print("Test program v1.0");
    string_array* array = split("abc def ghij klm"," ");
    for(uint16 i=0;i<array->length;i++){
        print(array->strings+array->positions[i]);
        print("  POS: ");
        printint(array->positions[i]);
        nextLine();
    }
    nextLine();
    removeStringFromArray(array,array->length-2);
    for(uint16 i=0;i<array->length;i++){
        print(array->strings+array->positions[i]);
        print("  POS: ");
        printint(array->positions[i]);
        nextLine();
    }

    free_string_array(array);
    return;
}

void terminal_program_test2(string_array* args){
    print("Test command #2");
    nextLine();
}

void pci_terminal_command(string_array* args){
    if(args->length==0){
        printPciInfo();
    }else{
        for(uint16 i=0;i<args->length;i++){
            if(memcmp(args->strings+args->positions[i],"-devices",8)){
                printPciDevices();
            }
        }
    }
}