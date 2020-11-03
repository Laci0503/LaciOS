#include "kernel.h"
uint8 video_mode = 0x3;

uint8 readConfusingRegister(uint8 index){
    inb(0x3da);
    outb(0x3c0,index);
    return inb(0x3c1);
}

void writeConfusingRegister(uint8 index, uint8 data){
    inb(0x3da);
    outb(0x3c0,index);
    outb(0x3c0,data);
}

uint8 readMiscPort(){
    return inb(0x3cc);
}
void writeMiscPort(uint8 data){
    outb(0x3c2,data);
    return;
}

uint8 readIndexed(uint8 port, uint8 index){
    switch (port)
    {
    case 0:
        outb(0x3c4,index);
        return inb(0x3c5);
    case 1:
        outb(0x3ce,index);
        return inb(0x3cf);
    case 2:
        outb(0x3d4,index);
        return inb(0x3d5);
    }
    return 0;
}
void writeIndexed(uint8 port, uint8 index, uint8 data){
    switch (port)
    {
    case 0:
        outb(0x3c4,index);
        outb(0x3c5,data);
        return;
    case 1:
        outb(0x3ce,index);
        outb(0x3cf,data);
        return;
    case 2:
        outb(0x3d4,index);
        outb(0x3d5,data);
        return;
    }
    return;
}

void setChain4(uint8 enabled){
    if(enabled){
        writeIndexed(0,0x4,readIndexed(0,0x4) | 0b00001000);
    }else{
        writeIndexed(0,0x4,readIndexed(0,0x4) & 0b11110111);
    }
    return;
}

void videoLoadSettings(uint8* settings){
    uint8* pointer = settings;
    writeMiscPort(*pointer);
    pointer++;
    for(uint8 i=0;i<5;i++){
        writeIndexed(0,i,*pointer);
        pointer++;
    }
    writeIndexed(2,0x3,readIndexed(2,0x3) | 0x80);
    writeIndexed(2,0x11,readIndexed(2,0x11) & ~0x80);

    pointer[0x3] |= 0x80;
    pointer[0x11] &= ~0x80;

    for(uint8 i=0;i<25;i++){
        writeIndexed(2,i,*pointer);
        pointer++;
    }

    for(uint8 i=0; i<9;i++){
        writeIndexed(1,i,*pointer);
        pointer++;
    }

    for(uint8 i=0;i<21;i++){
        writeConfusingRegister(i,*pointer);
        pointer++;
    }
    inb(0x3da);
    outb(0x3c0,0x20);
}

uint8 mode0x13[] = {
    /* MISC */
        0x63,
    /* SEQ */
        0x03, 0x01, 0x0F, 0x00, 0x0E,
    /* CRTC */
        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
        0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
        0xFF,
    /* GC */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
        0xFF,
    /* AC */
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x41, 0x00, 0x0F, 0x00, 0x00
};

void video_clear(){
    if(video_mode==0x13){
        uint64* pointer=(uint64*) 0xA0000;
        for(uint32 i=0;i<((320*200)/8);i++){
            pointer[i]=0;
        }
    }
}

void put_pixel(uint16 x, uint16 y, uint8 color){
    if(video_mode==0x13){
        *((uint8*)(0xA0000 + (uint32)y*320 + (uint32)x)) = color;
    }
}

void drawRectangleFilled(uint16 x, uint16 y, uint16 w, uint16 h, uint8 color){
    for(uint16 i=x;i<x+w;i++){
        for(uint16 j=y;j<y+h;j++){
            put_pixel(i,j,color);
        }
    }
}

void drawRectangle(uint16 x, uint16 y, uint16 w, uint16 h, uint8 color){
    drawRectangleFilled(x,y,1,h,color);
    drawRectangleFilled(x,y,w,1,color);
    drawRectangleFilled(x,y+h-1,w,1,color);
    drawRectangleFilled(x+w-1,y,1,h,color);
}

void drawLine(uint16 x1,uint16 y1,uint16 x2,uint16 y2,uint8 color){
    if(x1>x2){
        uint16 c=x1;
        x1=x2;
        x2=c;

        c=y1;
        y1=y2;
        y2=c;
    }

    double m=((double)y2-(double)y1)/((double)x2-(double)x1);

    if(m<=(double)1){
        for(uint16 i=0;i<x2-x1+1;i++){
            put_pixel(i+x1,y1+i*m,color);
        }
    }else{
        for(uint16 i=0;i<y2-y1+1;i++){
            put_pixel((double)x1+(double)i/m,y1+i,color);
        }
    }
}

void drawTriangle(uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 x3, uint16 y3, uint8 color){
    drawLine(x1,y2,x2,y2,color);
    drawLine(x1,y1,x3,y3,color);
    drawLine(x2,y2,x3,y3,color);
}

void switchMode(uint8 mode){
    if(mode==0x13){
        videoLoadSettings(mode0x13);
        video_mode=0x13;
        video_clear();
    }

}