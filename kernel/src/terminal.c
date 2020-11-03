#include "../headers/terminal.h"
#include "../headers/interrupts.h"
#include "../headers/video.h"
#include "../headers/memory.h"
#include "../headers/terminalPrograms.h"
#include "../headers/input.h"
#include "../headers/advanced_string.h"
#include "../headers/string.h"

char *keyboard_buffer;
uint8 keyboard_index=0;
uint8 command_running=0;
void keypress(uint8 key);

void runTerminal(){
    init_terminal_program_list();
    uint8 keyPressEventSlot = addKeyboardEvent(keypress);
    keyboard_buffer=malloc(81);
    goxy(0,24);
    forecolor=LIGHT_BLUE;
    backcolor=BLACK;
    print("_");
    goxy(0,24);
    while(1){
        if(keyboard_index!=0){
            if(keyboard_buffer[keyboard_index-1]==10){
                print(" ");
                nextLine();
                keyboard_buffer[keyboard_index-1]=0;
                string_array* array=split(keyboard_buffer," ");
                uint16 cmd_length=len(array->strings);
                uint32 cs = 0;
                for(uint8 i=0;i<cmd_length;i++){
                    cs+=*(array->strings+i);
                }
                for(uint16 i=0;i<program_count;i++){
                    if(Terminal_programs[i].name_checksum==cs){
                        if(memcmp(array->strings,Terminal_programs[i].name,cmd_length)){
                            forecolor=WHITE;
                            backcolor=BLACK;
                            removeStringFromArray(array,0);
                            command_running=1;
                            (*Terminal_programs[i].function_pointer)(array);
                            command_running=0;
                            forecolor=LIGHT_BLUE;
                            backcolor=BLACK;
                        }
                    }
                }
                free_string_array(array);
                goxy(0,24);
                print("_");
                goxy(0,24);
                keyboard_index=0;
            }
        }
    }
}
void keypress(uint8 key){
    if(!command_running){
        if((key & 0x80) == 0){
            if(key==28){
                if(keyboard_index!=0){
                    keyboard_buffer[keyboard_index]=10;
                    keyboard_index++;
                }
            }else if(ascii_keymap[key]!=0){
                if(keyboard_index!=79){
                    keyboard_buffer[keyboard_index]=ascii_keymap[key];
                    keyboard_index++;
                    write_letter(ascii_keymap[key],forecolor,backcolor);
                    print("_");
                    goxy(getX()-1,getY());
                }
            }else if(key==14){
                if(keyboard_index!=0){
                    keyboard_index--;
                    goxy(getX()-1,getY());
                    print("_ ");
                    goxy(getX()-2,getY());
                }
            }
        }
    }
}