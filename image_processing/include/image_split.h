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

typedef struct Line Line;

#endif //IMAGE_PROCESSING_IMAGE_SPLIT_H
