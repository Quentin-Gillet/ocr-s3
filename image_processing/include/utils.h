//
// Created by quentin on 06/10/22.
//

#ifndef OCR_UTILS_H
#define OCR_UTILS_H

#include <SDL2/SDL.h>
#include <string.h>
#include <err.h>
#include <stdlib.h>

typedef struct {
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
} hsv;

hsv rgb2hsv(rgb in);
rgb hsv2rgb(hsv in);

float truncate(float value, int val1, int val2);
char* stradd(const char* a, const char* b);
void insertionSort(double arr[], int n);

#endif
