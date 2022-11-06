//
// Created by alexiscognet on 05/11/22.
//

#ifndef IMAGE_PROCESSING_IMAGE_SPLIT_H
#define IMAGE_PROCESSING_IMAGE_SPLIT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utils.h"
#include "image.h"
#include "line_detection.h"

typedef struct Intersection{
    int x;
    int y;
    struct Line line;
} Intersection;

typedef struct LineIntersection{
    Intersection intersection;
    Line line;
} LineIntersection;

typedef struct Square{
    int xa;
    int ya;
    int xb;
    int yb;
    int xc;
    int yc;
    int xd;
    int yd;
} Square;

int approx(int a, int b);
Line* reduce_lines(Line* lines, int nbLines, int* newLinesCount);
Square* get_Squares(Line * lines, int NBLines);

/*
a _______ b
 |       |
 |       |
 |       |
c|_______|d
 */

#endif //IMAGE_PROCESSING_IMAGE_SPLIT_H
