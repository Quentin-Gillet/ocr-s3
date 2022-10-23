//
// Created by quentin on 06/10/22.
//

#ifndef OCR_LINE_DETECTION_H
#define OCR_LINE_DETECTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utils.h"

struct Line
{
    int x1;
    int x2;
    int y1;
    int y2;
};

struct Line* houghTransform(SDL_Surface* surface, float threshold, int* lineLength);

#endif
