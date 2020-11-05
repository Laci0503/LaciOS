#include "../headers/types.h"
#include "../headers/interrupts.h"
#include "../headers/memory.h"
#include "../headers/terminal.h"

#define kernel_length 0xF000

void mode_select_function(uint8 key);

uint8* mode_select_selected;
void _KERNEL_ENTRY(){
    /*videoInit();
    //clear();
    forecolor=WHITE;
    backcolor=BLACK;
    setupInterrupts();
    init_keymap();
    pciInit();
    mapPci();
    load_memory_map();
    memory_init();

    malloc(120*512);

    addKeyboardEvent(mode_select_function);

    goxy(0,10);
    print("Select mode: (Press 1 or 2 to select and enter to apply settings)");
    nextLine();
    forecolor=BLACK;
    backcolor=WHITE;
    print("> Text mode");
    nextLine();
    forecolor=WHITE;
    backcolor=BLACK;
    print("> Graphics mode");

    malloc(kernel_length);

    mode_select_selected=malloc(1);
    goxy(0,0);
    //printhex((uint32)mode_select_selected);
    *mode_select_selected=0;

    while((*mode_select_selected & 0x2) == 0);
    removeKeyboardEvent(0);
    clear();

    if(*mode_select_selected==0x2){
        free(mode_select_selected,1);
        runTerminal();
        
    }else{
        //graphics mode
    }*/

    //Memory manager
    //setupInterrupts();  //Interrupts
    //Task manager
        //Drivers
        //Terminal


    while(1);
}

/*void mode_select_function(uint8 key){
    goxy(0,0);
    if(key == 2){
        *mode_select_selected=0;
    }else if(key == 3){
        *mode_select_selected=1;
    }else if(key==28){
        *mode_select_selected|=0x2;
    }
    if(*mode_select_selected==0){
        goxy(0,11);
        forecolor=BLACK;
        backcolor=WHITE;
        print("> Text mode");
        nextLine();
        forecolor=WHITE;
        backcolor=BLACK;
        print("> Graphics mode");
    }else if(*mode_select_selected==1){
        goxy(0,11);
        forecolor=WHITE;
        backcolor=BLACK;
        print("> Text mode");
        nextLine();
        forecolor=BLACK;
        backcolor=WHITE;
        print("> Graphics mode");
    }
}*/