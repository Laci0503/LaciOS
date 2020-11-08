#include "../headers/types.h"
#include "../headers/interrupts.h"
#include "../headers/memory.h"
#include "../headers/terminal.h"
#include "../headers/taskmanager.h"
#include "../headers/drivers/video.h"
#include "../headers/string.h"

#define kernel_length 0xF000

//void mode_select_function(uint8 key);
void printInfinite(char c);
void printInfinite2(char c);
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
    //printInfinite('A');
    taskManagerRunning=0;
    load_memory_map();
    memory_init();
    malloc(120*512);
    setupInterrupts();  //Interrupts
    setupTaskManager();
    videoInit();
    forecolor=BLACK;
    backcolor=WHITE;
    //printInfinite('A');
    
    //Task manager
        //Drivers
        //Terminal
    int16 pid = addTask(&printInfinite2,100,0,"Test task","");
    *((uint32*)(tasks[pid]->savedCpuState.esp))='D';
    tasks[pid]->savedCpuState.esp-=4;
    print(tasks[pid]->name);
    print(" ");
    print(tasks[pid]->filename);
    tasks[pid]->state=TaskRunning;

    //if(numTasks==2)
    printInfinite('a');
    while(1);
}

void printInfinite(char c){
    uint32 i=80;
    while(1){
        *((uint8*)0xb8000+i*2)=c;
        *((uint8*)0xb8001+i*2)=0xf0;
        i++;
        for(uint16 j=0;j<100;j++){
            for(uint16 k=0;k<50000;k++){
                asm("NOP");
            }
        }
    };
}
void printInfinite2(char c){
    *((uint8*)0xb8000+1920*2)='C';
    *((uint8*)0xb8001+1920*2)=0xf0;
    uint32 i=2000;
    while(1){
        *((uint8*)0xb8000+i*2)=c;
        *((uint8*)0xb8001+i*2)=0xf0;
        i--;
        for(uint16 j=0;j<100;j++){
            for(uint16 k=0;k<50000;k++){
                asm("NOP");
            }
        }
    };
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