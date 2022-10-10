//
// Created by quentin on 06/10/22.
//

#ifndef OCR_IMAGE_PROCESSING_H
#define OCR_IMAGE_PROCESSING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utils.h"

enum ProcessingType
{
    GRAYSCALE,
    UP_CONTRAST,
    COLOR_INVERT
};

Uint32 pixelInvert(Uint32 pixelColor, SDL_PixelFormat* format);
void surfaceProcessing(SDL_Surface* surface, enum ProcessingType processingType);
Uint32 pixelBlackWhite(Uint32 pixelColor, SDL_PixelFormat* format);
Uint32 pixelToGrayScale(Uint32 pixelColor, SDL_PixelFormat* format);

#endif
