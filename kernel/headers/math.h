#ifndef _MATH_H
#define _MATH_H
#include "types.h"

#define PRINT_FLOAT_MAX_DECIMAL_DIGITS 10

uint32 pwr(uint32 base, uint8 exponent);
uint32 min(uint32 n1, uint32 n2);
uint32 max(uint32 n1, uint32 n2);
double root(double n);

struct cmx{
    double realPart;
    double imaginaryPart;
};

struct cmx* addCmx(struct cmx* cmx1, struct cmx* cmx2);
struct cmx* mulByReal(struct cmx* cmx1, double r);
struct cmx* mulByCmx(struct cmx* cmx1, struct cmx* cmx2);
struct cmx* cmxSq(struct cmx* cmx1);

void double_to_text(double n, char string[]);
void printFloat(double n);

#endif