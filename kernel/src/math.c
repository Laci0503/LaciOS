#include "../headers/math.h"
#include "../headers/memory.h"
#include "../headers/string.h"
#include "../headers/video.h"

uint32 pwr(uint32 base, uint8 exponent){
    uint8 exp = exponent-1;
    if(exponent==0)return 1;
    uint32 bs = base;
    while(exp != 0){
        bs *= base;
        exp--;
    }
    return bs;
}

uint32 min(uint32 n1, uint32 n2){
    if(n1 < n2)return n1; else return n2;
}

uint32 max(uint32 n1, uint32 n2){
    if(n1 > n2)return n1; else return n2;
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

struct cmx* addCmx(struct cmx* cmx1, struct cmx* cmx2){
    struct cmx* retval = (struct cmx*)malloc(sizeof(struct cmx));
    retval->realPart= cmx1->realPart + cmx2->realPart;
    retval->imaginaryPart = cmx1->imaginaryPart + cmx2->imaginaryPart;
    return retval;
}

struct cmx* mulByReal(struct cmx* cmx1, double r){
    struct cmx* retval = (struct cmx*)malloc(sizeof(struct cmx));
    retval->realPart= cmx1->realPart + r;
    retval->imaginaryPart = cmx1->imaginaryPart * r;
    return retval;
}

struct cmx* mulByCmx(struct cmx* cmx1, struct cmx* cmx2){
    struct cmx* retval = (struct cmx*)malloc(sizeof(struct cmx));
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

struct cmx* cmxSq(struct cmx* cmx1){
    return mulByCmx(cmx1, cmx1);
}

void double_to_text(double n, char string[]){
    uint32 int_part=(uint32) n;
    int_to_text(n,string);
    uint8 l = len(string);
    string[l]='.';
    l++;
    n-=(double)int_part;
    for(uint16 i=0;i<min(PRINT_FLOAT_MAX_DECIMAL_DIGITS,15-l-1);i++){
        uint8 digit=(uint8)(n*10.0);
        string[l+i]='0'+digit;
        n-=(double)digit/10.0;
        if((uint32)(n*pwr(10,PRINT_FLOAT_MAX_DECIMAL_DIGITS))==0)break;
        n*=10.0;
    }
    return;
}

void printFloat(double n){
    char buffer[15];
    for(uint8 i=0;i<15;i++){buffer[i]=0;}
    double_to_text(n,buffer);
    print(buffer);
    return;
}