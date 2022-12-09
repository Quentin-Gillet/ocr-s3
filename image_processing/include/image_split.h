#ifndef IMAGE_PROCESSING_IMAGE_SPLIT_H
#define IMAGE_PROCESSING_IMAGE_SPLIT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include "utils.h"
#include "image.h"
#include "line_detection.h"
#include "manual_rotation.h"

typedef struct Intersection{
    int x;
    int y;
    struct Line line;
} Intersection;

typedef struct Square{
    int xa;
    int xb;
    int xc;
    int xd;
    int ya;
    int yb;
    int yc;
    int yd;
    double perimeter;
} Square;

int approx(int a, int b, int Threshold);
Line* reduce_lines(Line* lines, int nbLines, int* newLinesCount);
struct Line* get_Bigger_Squares(struct Line* lines, int NBLine);
Image* split(struct Line *lines, Image *image, Image *cells);

struct Line* print_squares(struct Line* lines, int NBLine);

/*
a _______ b
 |       |
 |       |
 |       |
c|_______|d
 */

#endif