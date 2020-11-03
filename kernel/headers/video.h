#ifndef _VIDEO_H
#define _VIDEO_H
#include "types.h"

#define WIDTH 80
#define HEIGHT 25
#define VGA_ADDRESS 0xB8000

#define BLACK 0x0
#define DARK_BLUE 0x1
#define DARK_GREEN 0x2
#define DARK_AQUA 0x3
#define DARK_RED 0x4
#define PURPLE 0x5
#define DARK_YELLOW 0x6
#define LIGHT_GRAY 0x7
#define DARK_GRAY 0x8
#define LIGHT_BLUE 0x9
#define LIGHT_GREEN 0xa
#define LIGHT_AQUA 0xb
#define LIGHT_RED 0xc
#define MAGENTA 0xd
#define LIGHT_YELLOW 0xe
#define WHITE 0xf

char hexLetters[17];
uint8 video_mode;

uint16* TERMINAL_BUFFER;
uint8 forecolor;
uint8 backcolor;
uint16 vga_index;

void goxy(uint8 x, uint8 y);
void print(char text[]);
void nextLine();
void write_letter(char letter, uint8 fore_color, uint8 back_color);
uint8 readConfusingRegister(uint8 index);
void writeConfusingRegister(uint8 index, uint8 data);
uint8 readMiscPort();
void writeMiscPort(uint8 data);
uint8 readIndexed(uint8 port, uint8 index);
void writeIndexed(uint8 port, uint8 index, uint8 data);
void setChain4(uint8 enabled);
void videoLoadSettings(uint8* settings);
void video_clear();
void put_pixel(uint16 x, uint16 y, uint8 color);
void drawRectangleFilled(uint16 x, uint16 y, uint16 w, uint16 h, uint8 color);
void drawRectangle(uint16 x, uint16 y, uint16 w, uint16 h, uint8 color);
void drawLine(uint16 x1,uint16 y1,uint16 x2,uint16 y2,uint8 color);
void drawTriangle(uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 x3, uint16 y3, uint8 color);
void switchMode(uint8 mode);
uint8 getY();
uint8 getX();
void videoInit();
void clear();
void write_text(char text[], uint8 fore_color, uint8 back_color);

#endif