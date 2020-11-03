#ifndef _ADVANCED_STRING_H
#define _ADVANCED_STRING_H
#include "types.h"

typedef struct string_array{
    char* strings;
    uint32 length;
    uint32* positions;
} string_array;

char* replace_all(char source[], char substr[], char replace_value[]);
string_array* split(char source[], char delimeter[]);
void free_string_array(string_array* array);
void removeStringFromArray(string_array* array, uint32 index);
uint8 contains(char* src, char* substr);
double stringToDouble(char* string);
uint32 indexOf(char* src,char* substr);
int32 signedStringToInt(char* string);
#endif