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
    unsigned int pixelAverage;
} Pixel;

typedef struct Image
{
    int width;
    int height;
    Pixel **pixels;
} Image;

typedef struct ImageMajorOrder
{
    int width;
    int height;
    Pixel *pixels;
} ImageMajorOrder;

double bilinearInterpolation(unsigned int top, unsigned int bottom, unsigned int right, unsigned int left,
                             double horizontalPos, double verticalPos, Image* image);

Uint32 getPixel(SDL_Surface *surface, int x, int y);
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
SDL_Surface* crateSurfaceFromImage(Image* image);
Image createImageFromSurface(SDL_Surface* surface);
void setPixelSameValue(Pixel* pixel, Uint8 value);
void setPixelValue(Pixel* pixel, Uint8 r, Uint8 g, Uint8 b);
void calculatePixelAverage(Pixel* pixel);
void freeImage(Image* image);
Image copyImage(Image* image);
Pixel copyPixel(Pixel* pixel);
Image createImage(int width, int height);
ImageMajorOrder convertImageToMajorOrder(Image* image);

#endif //IMAGE_PROCESSING_IMAGE_H
