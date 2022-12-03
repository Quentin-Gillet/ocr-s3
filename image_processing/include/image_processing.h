//
// Created by quentin on 06/10/22.
//

#ifndef OCR_IMAGE_PROCESSING_H
#define OCR_IMAGE_PROCESSING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "utils.h"

enum ProcessingType
{
    GRAYSCALE,
    BLACK_WHITE,
    UP_CONTRAST,
    COLOR_INVERT
};

Uint32 pixelInvert(Uint32 pixelColor, SDL_PixelFormat* format);
Uint32 pixelContrast(Uint32 pixel_color, SDL_PixelFormat* format);
void surfaceProcessing(SDL_Surface* surface, enum ProcessingType processingType);
Uint32 pixelBlackWhite(Uint32 pixelColor, SDL_PixelFormat* format);
Uint32 pixelToGrayScale(Uint32 pixelColor, SDL_PixelFormat* format);
void surfaceMedianBlur(SDL_Surface* surface);
Uint32 getPixel(SDL_Surface *surface, int x, int y);
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
Uint32* getAdjacentPixels(SDL_Surface* surface, int x, int y);
Uint32 getPixelMedian(SDL_PixelFormat* format, Uint32* pixels);
void surfaceSobelFilter(SDL_Surface* surface);
double convolution(SDL_Surface* surface, double kernel[3][3], int row, int col);
void surfaceBinarisaion(SDL_Surface* surface);
int otsuMethod(SDL_Surface* surface);

#endif
