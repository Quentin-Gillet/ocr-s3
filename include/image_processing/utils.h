//
// Created by quentin on 06/10/22.
//

#ifndef OCR_UTILS_H
#define OCR_UTILS_H

#include <SDL2/SDL.h>
#include <string.h>
#include <err.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

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

float clampFloat(float value, float min, float max);
Uint8 clampUint8(Uint8 value, Uint8 min, Uint8 max);
int clampIntMin(int value, int min);
char* stradd(const char* a, const char* b);
void insertionSort(double arr[], int n);
char* concat(const char* str1, const char* str2);
int clampInt(int value, int min, int max);

unsigned int **initMatrix(unsigned int x, unsigned int y);
void freeMatrix(unsigned int **matrice, size_t height);

#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

#endif
