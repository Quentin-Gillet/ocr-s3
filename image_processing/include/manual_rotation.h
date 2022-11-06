#ifndef OCR_MANUAL_ROTATION_H
#define OCR_MANUAL_ROTATION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "utils.h"
#include "image.h"

double bilinearInterpolation(unsigned int top, unsigned int bottom, unsigned int right, unsigned int left,
                             double horizontalPos, double verticalPos, Image* image);
void imageRotate(Image* image, int angleDegree);

#endif
