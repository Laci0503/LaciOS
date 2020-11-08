#include "../headers/string.h"
#include "../headers/drivers/video.h"
#include "../headers/math.h"

uint8 strcmp(char str1[], char str2[]){
    if(len(str1) == len(str2)){
        for(uint16 i=0; i<len(str1); i++){
            if(str1[i]!=str2[i]) return 0;
        }
        return 1;
    }else return 0;
}

uint16 len(char text[]){
    uint16 l = 0;
    while (text[l]!=0){
        l++;
    }
    return l;
}

void int_to_text(uint32 n, char string[]){
    uint32 div=n;
    uint8 index=0;
    if(div==0){
        string[0]='0';
        return;
    }
    char buffer[21];
    while (div>0){
        buffer[index]=div%10+'0';
        div=(uint32)div/10;
        index++;
    }
    for(uint8 i=0;i<21;i++){string[i]=0;}
    for(uint8 i=0;i<index;i++){
        string[i]=buffer[index-i-1];
    }
    return;
}

void int_to_text_hex(uint32 n, char string[]){
    uint32 div=n;
    uint8 index=0;
    if(div==0){
        string[0]='0';
        return;
    }
    char buffer[21];
    while (div>0){
        buffer[index]=*(hexLetters+div%16);
        div=(uint32)div/16;
        index++;
    }
    for(uint8 i=0;i<21;i++){string[i]=0;}
    for(uint8 i=0;i<index;i++){
        string[i]=buffer[index-i-1];
    }
    return;
}

void int_to_text_bin(uint32 n, char string[]){
    uint32 div=n;
    uint8 index=0;
    if(div==0){
        string[0]='0';
        return;
    }
    char buffer[33];
    while (div>0){
        buffer[index]=*(hexLetters+div%2);
        div=(uint32)div/2;
        index++;
    }
    for(uint8 i=0;i<33;i++){string[i]=0;}
    for(uint8 i=0;i<index;i++){
        string[i]=buffer[index-i-1];
    }
    return;
}

void print_int(uint64 number, uint8 fore_color, uint8 back_color){
    char buffer[21];
    for(uint8 i=0;i<11;i++){buffer[i]=0;}
    int_to_text(number, buffer);
    write_text(buffer, fore_color, back_color);
    return;
}

void printint(uint32 number){
    char buffer[21];
    for(uint8 i=0;i<11;i++){buffer[i]=0;}
    int_to_text(number, buffer);
    write_text(buffer, forecolor, backcolor);
    return;
}

void printhex(uint32 number){
    char buffer[21];
    for(uint8 i=0;i<21;i++){buffer[i]=0;}
    int_to_text_hex(number, buffer);
    write_text(buffer, forecolor, backcolor);
    return;
}

void printbin(uint32 number){
    char buffer[33];
    for(uint8 i=0;i<33;i++){buffer[i]=0;}
    int_to_text_bin(number, buffer);
    write_text(buffer, forecolor, backcolor);
    return;
}

void printsignedint(int32 number){
    char buffer[21];
    for(uint8 i=0;i<11;i++){buffer[i]=0;}
    uint32 n;
    if(number<0){
        n=-1*number;
        buffer[0]='-';
        char* t = buffer;
        t++;
        int_to_text(n, t);
    }else{
        n=number;
        int_to_text(n, buffer);
    }
    write_text(buffer, forecolor, backcolor);
    return;
}

uint32 stringToInt(char* string){
    uint32 length=len(string);
    if(length==0){
        return 0;
    }
    uint32 number=0;
    uint32 tmp=0;
    for(uint8 i=0;i<length;i++){
        if(string[i]>='0' && string[i]<='0'+9){
            tmp=string[i]-'0';
            for(uint8 j=1;j<length-i;j++){
                tmp*=10;
            }
            number+=tmp;
        }else{
            return 0;
        }
    }
    return number;
}
int32 signedStringToInt(char* string){
    uint32 length=len(string);
    if(length==0){
        return 0;
    }
    int32 number=0;
    int32 tmp=0;
    for(uint8 i=(string[0]=='-');i<length;i++){
        if(string[i]>='0' && string[i]<='0'+9){
            tmp=string[i]-'0';
            for(uint8 j=1;j<length-i;j++){
                tmp*=10;
            }
            number+=tmp;
        }else{
            return 0;
        }
    }
    return number * (string[0]=='-' ? -1 : 1);
}