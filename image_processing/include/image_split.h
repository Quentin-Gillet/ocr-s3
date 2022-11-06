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

int approx(int a, int b);
struct Line* reduce_lines(struct Line* lines, int nbLines );
struct Square* get_Squares(struct Line * lines, int NBLines);

typedef struct Line Line;

typedef struct Intersection{
    int x;
    int y;
    struct Line line;
}Intersection;

struct LineIntersection{
    Intersection intersection;
    Line line;
};

struct Square{
    int xa;
    int ya;
    int xb;
    int yb;
    int xc;
    int yc;
    int xd;
    int yd;
};
/*
a _______ b
 |       |
 |       |
 |       |
c|_______|d
 */

#endif //IMAGE_PROCESSING_IMAGE_SPLIT_H
