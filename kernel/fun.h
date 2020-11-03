#include "kernel.h"
#include "math.h"

uint16 maxiter = 50;

struct cmx{
    double realPart;
    double imaginaryPart;
};

struct cmx* addCmx(struct cmx* cmx1, struct cmx* cmx2){
    struct cmx* retval = malloc(sizeof(struct cmx));
    retval->realPart= cmx1->realPart + cmx2->realPart;
    retval->imaginaryPart = cmx1->imaginaryPart + cmx2->imaginaryPart;
    return retval;
}

struct cmx* mulByReal(struct cmx* cmx1, double r){
    struct cmx* retval = malloc(sizeof(struct cmx));
    retval->realPart= cmx1->realPart + r;
    retval->imaginaryPart = cmx1->imaginaryPart * r;
    return retval;
}

struct cmx* mulByCmx(struct cmx* cmx1, struct cmx* cmx2){
    struct cmx* retval = malloc(sizeof(struct cmx));
    double a = cmx1->realPart * cmx2->realPart;
    double b = cmx1->realPart * cmx2->imaginaryPart;
    double c = cmx1->imaginaryPart * cmx2->realPart;
    double d = cmx1->imaginaryPart * cmx2->imaginaryPart;
    b=b+c;
    c=d;
    c=c*-1;
    a=a+c;
    retval->realPart=a;
    retval->imaginaryPart=b;
    return retval;
}

struct cmx* sq(struct cmx* cmx1){
    return mulByCmx(cmx1, cmx1);
}

double root(double n){
  double lo = 0, hi = n, mid;
  for(int i = 0 ; i < 1000 ; i++){
      mid = (lo+hi)/2;
      if(mid*mid == n) return mid;
      if(mid*mid > n) hi = mid;
      else lo = mid;
  }
  return mid;
}

uint16 iter(struct cmx* complex){
    uint16 j=1;
    struct cmx x;
    x.imaginaryPart=complex->imaginaryPart;
    x.realPart=complex->realPart;
    while(root(pwr(x.realPart,2) + pwr(x.imaginaryPart,2)) < 2.0 && j<maxiter){
        struct cmx* t = sq(&x);
        struct cmx* t2 = addCmx(t,complex);
        x.imaginaryPart=t2->imaginaryPart;
        x.realPart=t2->realPart;
        free(t,sizeof(struct cmx));
        free(t2,sizeof(struct cmx));
        j++;
    }
    return j;
}

void drawMandelbrot(){
    switchMode(0x13);
    //video_clear();
    drawRectangle(0,0,319,299,0x4);
    double cx=-0.2;
    double cy=0.0;
    double zoom=1.5;
    uint16 h=200;
    uint16 wmargin=(320-200)/2;
    char asd[5];
    for(uint16 k=0;k<=h;k++){
        for(uint16 l=0;l<=h;l++){
            double a=(((double)l/(double)h-0.5)*2.0)*(double)zoom+cx;
            double b=(((double)k/(double)h-0.5)*2.0)*(double)zoom-cy;
            struct cmx t;
            t.realPart=a;
            t.imaginaryPart=b;
            uint16 i=iter(&t);
            if(i==maxiter){
                put_pixel(wmargin+l,k,WHITE);
            }else{
                if(i<(maxiter/4)){
                    put_pixel(wmargin+l,k,LIGHT_BLUE);
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
    //drawLine(wmargin,h/2,wmargin+h,h/2,LIGHT_GREEN);
    while(1)asm("NOP");
}

void test(){
    
}