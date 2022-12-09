//
// Created by quentin on 06/10/22.
//

#ifndef OCR_LINE_DETECTION_H
#define OCR_LINE_DETECTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utils.h"
#include "image.h"


typedef struct Line
{
    int x1;
    int y1;
    int x2;
    int y2;
    int theta;
} Line;

void drawLineOnImage(Image* image, Line* lines, int linesLength);
struct Line* getImageLines(Image *image, int threshold, int *lineLength);

#endif
