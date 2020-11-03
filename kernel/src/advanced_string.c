#include "../headers/advanced_string.h"
#include "../headers/string.h"
#include "../headers/memory.h"
#include "../headers/video.h"
#include "../headers/math.h"

char* replace_all(char source[], char substr[], char replace_value[]){
    uint16 repval_length=len(replace_value);
    uint16 source_length=len(source);
    uint16 substr_length=len(substr);

    if(substr_length==0){
        char* buffer=malloc(source_length+1);
        memcpy(source, buffer, source_length+1);
        return buffer;
    }

    uint16* rep_pointers=(uint16*)malloc((source_length / substr_length + 1)*2);
    uint16 index=0;

    for(uint16 i=0;i<source_length-substr_length;i++){
        if(memcmp(source+i,substr,substr_length)){
            rep_pointers[index]=i;
            index++;
        }
    }

    char *buffer;
    uint16 temp=source_length;
    temp-=index*substr_length;
    temp+=index*repval_length;
    buffer=malloc(temp+1);
    buffer[temp]=0;
    uint16 substr_count=index;
    index=0;

    for(uint16 i=0;i<substr_count;i++){
        memcpy(source+(i==0 ? 0 : rep_pointers[i-1] + substr_length),buffer+index,(i==0 ? rep_pointers[i] : rep_pointers[i] - rep_pointers[i-1] - substr_length));
        index+=(i==0 ? rep_pointers[i] : rep_pointers[i] - rep_pointers[i-1] - substr_length);
        memcpy(replace_value,buffer+index,repval_length);
        index+=repval_length;
    }
    memcpy(source+(substr_count==0 ? 0 : rep_pointers[substr_count-1]+substr_length),buffer+index,source_length-rep_pointers[substr_count-1]);

    free((uint8*)rep_pointers,source_length);

    return buffer;
}

string_array* split(char source[], char delimeter[]){
    uint16 source_length=len(source);
    uint16 delim_length=len(delimeter);

    if(delim_length==0){
        char* buffer=(char*)malloc(source_length+1);
        memcpy(source, buffer, source_length+1);
        string_array* array=(string_array*)malloc(sizeof(string_array));
        array->strings=buffer;
        array->length=1;
        array->positions=(uint32*)malloc(4);
        *array->positions=0;
        return array;
    }
    uint32 index=0;
    uint32 delim_malloc_length=(source_length/delim_length+1)*4;
    uint32* delim_positions=(uint32*)malloc(delim_malloc_length);

    for(uint32 i=0;i<source_length-delim_length;i++){
        if(memcmp(source+i,delimeter,delim_length)){
            delim_positions[index]=i;
            index++;
        }
    }
    uint32 tmp=source_length-((index-1)*delim_length)+index+1;
    char* buffer=(char*)malloc(tmp);

    uint32 delim_count=index;
    string_array* array= (string_array*)malloc(sizeof(string_array));
    array->length=delim_count+1;
    array->strings=buffer;
    array->positions=(uint32*)malloc((delim_count+1)*4);
    index=0;
    for(uint32 i=0;i<delim_count;i++){
        array->positions[i]=index;
        memcpy(source+(i==0 ? 0 : delim_positions[i-1] + delim_length),buffer+index,(i==0 ? delim_positions[i] : delim_positions[i] - delim_positions[i-1] - delim_length));
        index+=(i==0 ? delim_positions[i] : delim_positions[i] - delim_positions[i-1] - delim_length);
        buffer[index]=0;
        index++;
    }
    array->positions[delim_count]=index;

    memcpy(source+(delim_count==0 ? 0 : delim_positions[delim_count-1]+delim_length),buffer+index,source_length-delim_positions[delim_count-1]);
    index+=source_length-delim_positions[delim_count-1];
    buffer[index++]=0;
    free((uint8*)delim_positions,delim_malloc_length);
    return array;
}

void removeStringFromArray(string_array* array, uint32 index){
    if(index>=array->length){
        return;
    }
    if(index==array->length-1){
        uint32 itemlength = len(array->strings+array->positions[index])+1;
        *(array->strings+array->positions[index]) = 0;
        free((uint8*)array->strings+array->positions[index]+1,itemlength);
        array->length--;
        free((uint8*)array->positions+(4*index),4);
    }else{
        uint32 itemlength = len(array->strings+array->positions[index])+1;
        memcpy(array->strings+array->positions[index+1],array->strings+array->positions[index],1+array->positions[array->length-1]-array->positions[index+1]+len(array->strings+array->positions[array->length-1])+1);
        for(uint32 i=index+1;i<array->length-1;i++){
            array->positions[i]=array->positions[i+1]-itemlength;
        }
        array->length--;
        free((uint8*)array->positions+array->length,4);
        free((uint8*)array->strings+array->positions[array->length-1]+len(array->strings+array->positions[array->length-1])+2,itemlength);
    }
}

void free_string_array(string_array* array){
    uint32 strings_length=1;
    for(uint32 i=0;i<array->length;i++){
        strings_length+=len(array->strings+array->positions[i])+1;
    }
    free((uint8*)array->strings,strings_length);
    free((uint8*)array->positions,array->length*4);
    free((uint8*)array,sizeof(string_array));
}

uint8 contains(char* src, char* substr){
    uint32 srclen=len(src);
    uint32 substrlen=len(substr);

    if(substrlen>srclen)return 0;
    if(substrlen==0)return 1;
    if(srclen==0)return 0;

    for(uint32 i=0;i<=srclen-substrlen;i++){
        if(memcmp(src+i,substr,substrlen)){
            return 1;
        }
    }
    return 0;
}

uint32 indexOf(char* src,char* substr){
    uint32 srclen=len(src);
    uint32 substrlen=len(substr);

    if(substrlen>srclen)return 0;
    if(substrlen==0)return 1;
    if(srclen==0)return 0;

    for(uint32 i=0;i<=srclen-substrlen;i++){
        if(memcmp(src+i,substr,substrlen)){
            return i;
        }
    }
    return 0;
}

double stringToDouble(char* string){
    uint32 stringlength=len(string);
    if(stringlength==0)return 0;
    if(contains(string,".") ^ contains(string,",")){
        
        double number=0;
        uint16 decimalPosition=indexOf(string,".")+indexOf(string,",");
        uint8 tmp=0;
        for(uint8 i=(string[0]=='-' ? 1 : 0);i<stringlength;i++){
            if(string[i]>='0' && string[i]<=('0'+9)){
                tmp=string[i]-'0';
                tmp*=pwr(10,decimalPosition-i-1);
                number+=tmp;
            }else if (string[i]=='.' || string[i]==','){
                decimalPosition=i;
                break;
            }else{
                return (double)0;
            }
        }

        for(uint16 i=decimalPosition+1;i<stringlength;i++){
            if(string[i]>='0' && string[i]<='0'+9){
                number+=(1.0/((double)pwr(10,i-decimalPosition)))*(double)(string[i]-'0');
            }else{
                return (double)0;
            }
        }
        return number * (string[0]=='-' ? -1.0 : 1.0);
    }else{
        return (double)signedStringToInt(string);
    }
}