#include "../headers/mandelbrot.h"
#include "../headers/math.h"
#include "../headers/video.h"
#include "../headers/advanced_string.h"
#include "../headers/memory.h"
#include "../headers/string.h"
#include "../headers/interrupts.h"

uint16 iter(struct cmx* complex,uint16 maxiter){
    uint16 j=1;
    struct cmx x;
    x.imaginaryPart=complex->imaginaryPart;
    x.realPart=complex->realPart;
    while(root(pwr(x.realPart,2) + pwr(x.imaginaryPart,2)) < 2.0 && j<maxiter){
        struct cmx* t = cmxSq(&x);
        struct cmx* t2 = addCmx(t,complex);
        x.imaginaryPart=t2->imaginaryPart;
        x.realPart=t2->realPart;
        free((uint8*)t,sizeof(struct cmx));
        free((uint8*)t2,sizeof(struct cmx));
        j++;
    }
    return j;
}

uint8 exitSignal=0;

void exitKeyboardListener(uint8 key){
    if(key==0x1){
        exitSignal=1;
    }
}

void drawMandelbrot(uint16 maxiter, double cx, double cy, double zoom){
    switchMode(0x13);
    exitSignal=0;
    addKeyboardEvent(exitKeyboardListener);
    //drawRectangle(0,0,320,200,0x4);
    /*double cx=-0.2;
    double cy=0.0;
    double zoom=1.5;*/
    drawRectangleFilled(0,0,320,200,LIGHT_BLUE);
    uint16 h=200;
    uint16 wmargin=(320-200)/2;
    for(uint16 k=0;k<=h;k++){
        for(uint16 l=0;l<=h;l++){
            if(exitSignal){
                switchMode(0x03);
                return;
            }
            double a=(((double)l/(double)h-0.5)*2.0)*(double)zoom+cx;
            double b=(((double)k/(double)h-0.5)*2.0)*(double)zoom-cy;
            struct cmx t;
            t.realPart=a;
            t.imaginaryPart=b;
            uint16 i=iter(&t,maxiter);
            if(i==maxiter){
                put_pixel(wmargin+l,k,BLACK);
            }else{
                if(i<(maxiter/4)){
                    
                }else if(i<(maxiter/2)){
                    put_pixel(wmargin+l,k,DARK_BLUE);
                }else if(i<((maxiter/4)*3)){
                    put_pixel(wmargin+l,k,LIGHT_RED);
                }else{
                    put_pixel(wmargin+l,k,DARK_RED);
                }
            }
            goxy(0,0);
            printint(i);
        }
    }
    uint8 clh=cy*((double)h/(zoom*2.0)) + (double)(h/2);
    drawLine(wmargin,clh,wmargin+h,clh,LIGHT_GREEN);
    while(!exitSignal)asm("NOP");
    switchMode(0x03);
    return;
}

void MandelbrotConsoleCommand(string_array* args){
    uint16 maxiter=50;
    double cx=0;
    double cy=0;
    double zoom=2;
    for(uint16 i=0;i<args->length;i++){
        if(len(args->strings+args->positions[i])==6){
            if(memcmp(args->strings+args->positions[i],"--help",6)){
                print("mandelbrot command draws the Mandelbrot set.");
                nextLine();
                print("Options:");
                nextLine();
                print(" --help: shows this help page");
                nextLine();
                print(" -maxiter <num>: sets the maximum number of iterations");
                nextLine();
                print(" -cx <num>: sets the real part of the center location");
                nextLine();
                print(" -cy <num>: sets the imaginary part of the center location");
                nextLine();
                print(" -zoom <num>: sets the distance from the left-most side to the right-most side of the real-part numberline");
                nextLine();
                print("Default values: maxiter: 50, cx: 0, cy: 0, zoom: 2");
                nextLine();
                return;
            }
        }
        if(len(args->strings+args->positions[i])==8){
            if(memcmp(args->strings+args->positions[i],"-maxiter",8)){
                if(i!=args->length-1){
                    maxiter=stringToInt(args->strings+args->positions[i+1]);
                    i++;
                }
            }
        }
        if(len(args->strings+args->positions[i])==3){
            if(memcmp(args->strings+args->positions[i],"-cx",3)){
                if(i!=args->length-1){
                    cx=stringToDouble(args->strings+args->positions[i+1]);
                    i++;
                }
            }
        }
        if(len(args->strings+args->positions[i])==3){
            if(memcmp(args->strings+args->positions[i],"-cy",3)){
                if(i!=args->length-1){
                    cy=stringToDouble(args->strings+args->positions[i+1]);
                    i++;
                }
            }
        }
        if(len(args->strings+args->positions[i])==5){
            if(memcmp(args->strings+args->positions[i],"-zoom",5)){
                if(i!=args->length-1){
                    zoom=stringToDouble(args->strings+args->positions[i+1]);
                    i++;
                }
            }
        }
    }
    drawMandelbrot(maxiter,cx,cy,zoom);
}