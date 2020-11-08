#include"kernel.h"



void init_keymap(){
    for(uint8 i=0;i<128;i++){
        ascii_keymap[i]=0;
    }
    ascii_keymap[41]='0';
    ascii_keymap[2]='1';
    ascii_keymap[3]='2';
    ascii_keymap[4]='3';
    ascii_keymap[5]='4';
    ascii_keymap[6]='5';
    ascii_keymap[7]='6';
    ascii_keymap[8]='7';
    ascii_keymap[9]='8';
    ascii_keymap[10]='9';

    ascii_keymap[16]='q';
    ascii_keymap[17]='w';
    ascii_keymap[18]='e';
    ascii_keymap[19]='r';
    ascii_keymap[20]='t';
    ascii_keymap[21]='z';
    ascii_keymap[22]='u';
    ascii_keymap[23]='i';
    ascii_keymap[24]='o';
    ascii_keymap[25]='p';

    ascii_keymap[30]='a';
    ascii_keymap[31]='s';
    ascii_keymap[32]='d';
    ascii_keymap[33]='f';
    ascii_keymap[34]='g';
    ascii_keymap[35]='h';
    ascii_keymap[36]='j';
    ascii_keymap[37]='k';
    ascii_keymap[38]='l';

    ascii_keymap[44]='y';
    ascii_keymap[45]='x';
    ascii_keymap[46]='c';
    ascii_keymap[47]='v';
    ascii_keymap[48]='b';
    ascii_keymap[49]='n';
    ascii_keymap[50]='m';
    ascii_keymap[51]=',';
    ascii_keymap[52]='.';
    ascii_keymap[53]='-';

    ascii_keymap[57]=32;

    for(uint8 i=0;i<128;i++){
        shift_ascii_keymap[i]=ascii_keymap[i];
    }

    for(uint8 i =16;i<26;i++){
        shift_ascii_keymap[i]=ascii_keymap[i]-0x20;
    }
    for(uint8 i =30;i<39;i++){
        shift_ascii_keymap[i]=ascii_keymap[i]-0x20;
    }
    for(uint8 i =44;i<51;i++){
        shift_ascii_keymap[i]=ascii_keymap[i]-0x20;
    }
    shift_ascii_keymap[51]='?';
    shift_ascii_keymap[52]=':';
    shift_ascii_keymap[53]='_';

    shift_ascii_keymap[41]=0;
    shift_ascii_keymap[2]=0x27;
    shift_ascii_keymap[3]='"';
    shift_ascii_keymap[4]='+';
    shift_ascii_keymap[5]='!';
    shift_ascii_keymap[6]='%';
    shift_ascii_keymap[7]='/';
    shift_ascii_keymap[8]='=';
    shift_ascii_keymap[9]='(';
    shift_ascii_keymap[10]=')';


    /**ascii_keymap[11]='ö';
    *ascii_keymap[12]='ü';
    *ascii_keymap[13]='ó';
    *ascii_keymap[26]='ő';
    *ascii_keymap[27]='ú';
    *ascii_keymap[39]='é';
    *ascii_keymap[40]='á';
    *ascii_keymap[43]='ű';*/
}

uint8 next_keyboard_byte(){
    return inb(KEYBOARD_PORT);
}

void input_test(){
    uint8 inp_code=0;
    uint8 last_code=0;
    uint8 code=0;
    uint8 shift=0;
    char buf[2];
    buf[1]=0;
    while(1){
        inp_code=next_keyboard_byte();
        if(inp_code!=last_code){
            code=inp_code & 0b01111111;
            if(ascii_keymap[code]!=0 && !(inp_code>>7)){
                if(shift){
                    buf[0]=shift_ascii_keymap[code];
                }else{
                    buf[0]=ascii_keymap[code];
                }
                print(buf);
            }
            if(code==42 || code==54){
                shift=!(inp_code>>7);
            }
            
            last_code=inp_code;
        }
    }
}

void read(char buffer[],uint8 count){
    uint8 inp_code=0;
    uint8 last_code=0;
    uint8 code=0;
    uint8 index=0;
    uint16 save[WIDTH];
    uint16 save2[WIDTH];
    //uint16 save3[WIDTH];
    uint8 shift=0;

    uint16 start=(HEIGHT/2-1)*WIDTH+0;

    for(uint16 i=start;i<start+WIDTH;i++){
        save[i-start]=TERMINAL_BUFFER[i];
    }
    for(uint16 i=start+WIDTH;i<start+2*WIDTH;i++){
        save2[i-start-WIDTH]=TERMINAL_BUFFER[i];
    }
    for(uint16 i=start+2*WIDTH;i<start+3*WIDTH;i++){
        save2[i-start-2*WIDTH]=TERMINAL_BUFFER[i];
    }
    

    for(uint16 i = start; i<start+WIDTH;i++){
        TERMINAL_BUFFER[i]=((DARK_GRAY<<4 | WHITE) << 8) | ' ';
    }

    for(uint16 i = start+WIDTH; i<start+2*WIDTH;i++){
        TERMINAL_BUFFER[i]=((LIGHT_GRAY<<4 | WHITE) << 8) | ' ';
    }

    for(uint16 i = start+2*WIDTH; i<start+3*WIDTH;i++){
        TERMINAL_BUFFER[i]=((DARK_GRAY<<4 | WHITE) << 8) | ' ';
    }

    TERMINAL_BUFFER[start+WIDTH]=((DARK_GRAY<<4 | WHITE) << 8) | ' ';
    TERMINAL_BUFFER[start+2*WIDTH-1]=((DARK_GRAY<<4 | WHITE) << 8) | ' ';
    TERMINAL_BUFFER[start+WIDTH+1]=((LIGHT_GRAY<<4 | BLACK) << 8) | '_';

    vga_index=start+WIDTH+1;
    inp_code=inb(KEYBOARD_PORT);
    while(last_code!=inp_code){
        last_code=inp_code;
        inp_code=inb(KEYBOARD_PORT);
    }
    index=0;
    while ((index<count || count==0) && (inp_code!=28 || last_code==inp_code)){
        last_code=inp_code;
        inp_code=next_keyboard_byte();
        if(inp_code!=last_code){
            code=inp_code & 0b01111111;
            if(ascii_keymap[code]!=0 && !(inp_code>>7)){
                if(shift){
                    buffer[index]=shift_ascii_keymap[code];
                }else{
                    buffer[index]=ascii_keymap[code];
                }
                write_letter(buffer[index],BLACK,LIGHT_GRAY);
                write_letter('_',BLACK,LIGHT_GRAY);
                vga_index--;
                index++;
            }
            if(code==42 || code==54){
                shift=!(inp_code>>7);
            }
            if(code==33 && !(inp_code>>7)){
                vga_index--;
                write_letter('_',BLACK, LIGHT_GRAY);
                vga_index--;
                index--;
            }
        }
    }

    TERMINAL_BUFFER[24*80]=((DARK_GRAY<<4 | WHITE) << 8) | '!';

    for(uint16 i=start;i<start+WIDTH;i++){
        TERMINAL_BUFFER[i]=save[i-start];
    }
    for(uint16 i=start+WIDTH;i<start+2*WIDTH;i++){
        TERMINAL_BUFFER[i]=save2[i-start-WIDTH];
    }
    for(uint16 i=start+2*WIDTH;i<start+3*WIDTH;i++){
        TERMINAL_BUFFER[i]=save2[i-start-2*WIDTH];
    }
    TERMINAL_BUFFER[start]=save[0];
    //TERMINAL_BUFFER[start+1]=save2[0];
    //TERMINAL_BUFFER[start+2]=save3[0];

    return;
}



uint8 testinputinterruptlast_code;
uint8 testinputinterrupt_event_slot;

void testinputinterrupt(uint8 key){
    if(key!=testinputinterruptlast_code){
        uint8 code=key & 0b01111111;
        if(ascii_keymap[code]!=0 && !(key>>7)){
            write_letter(ascii_keymap[code],forecolor,backcolor);
        }else if(code==0x1){
            removeKeyboardEvent(testinputinterrupt_event_slot);
        }else if(code==28 && !(key>>7)){
            nextLine();
        }
    }
    testinputinterruptlast_code=key;
    return;
}

void test_input_interrupt(){
    testinputinterrupt_event_slot = addKeyboardEvent(testinputinterrupt);
}

