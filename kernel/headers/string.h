#ifndef _STRING_H
#define _STRING_H
#include "types.h"

void int_to_text(uint32 n, char string[]);
void int_to_text_hex(uint32 n, char string[]);
void int_to_text_bin(uint32 n, char string[]);
uint8 strcmp(char str1[], char str2[]);
uint16 len(char text[]);
void print_int(uint64 number, uint8 fore_color, uint8 back_color);
void printint(uint32 number);
void printhex(uint32 number);
void printbin(uint32 number);
void printsignedint(int32 number);

uint32 stringToInt(char* string);

#endif