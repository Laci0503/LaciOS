#ifndef _MANDELBROT_H
#define _MANDELBROT_H

#include "../headers/types.h"
#include "../headers/math.h"
#include "../headers/advanced_string.h"

uint16 iter(struct cmx* complex,uint16 maxiter);
void drawMandelbrot(uint16 maxiter, double cx, double cy, double zoom);
void MandelbrotConsoleCommand(string_array* args);

#endif