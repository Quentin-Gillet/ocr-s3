//
// Created by Quentin Gillet on 26/10/2022.
//

#ifndef IMAGE_PROCESSING_IMAGE_H
#define IMAGE_PROCESSING_IMAGE_H

#include <SDL2/SDL.h>
#include <err.h>

typedef struct Pixel
{
    unsigned int r, g, b;
    Uint8 pixelAverage;
} Pixel;

typedef struct Image
{
    const unsigned int width;
    const unsigned int height;
    Pixel **pixels;
} Image;

Uint32 getPixel(SDL_Surface *surface, int x, int y);
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
SDL_Surface* crateSurfaceFromImage(Image* image);
Image newImage(SDL_Surface* surface);
void setPixelSameValue(Pixel* pixel, unsigned int value);
void setPixelValue(Pixel* pixel, Uint8 r, Uint8 g, Uint8 b);
void calculatePixelAverage(Pixel* pixel);
void freeImage(Image* image);

#endif //IMAGE_PROCESSING_IMAGE_H
