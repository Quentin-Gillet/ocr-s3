#include "../include/image_processing.h"

// Invert pixel color
Uint32 pixelInvert(Uint32 pixelColor, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixelColor, format, &r, &g, &b);
    return SDL_MapRGB(format, 255 - r, 255 - g, 255 - b);
}

Uint32 pixelBlackWhite(Uint32 pixelColor, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixelColor, format, &r, &g, &b);
    if((r + g + b) / 3 >= 127)
    {
        r = 255;
        g = 255;
        b = 255;    
    }
    else
    {
        r = 0;
        g = 0;
        b = 0;
    }
    Uint32 color = SDL_MapRGB(format, r, g, b);
    return color;
}

// Converts a colored pixel into grayscale.
//
// pixelColor: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixelToGrayScale(Uint32 pixelColor, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixelColor, format, &r, &g, &b);
    Uint8 average = 0.3*r + 0.59*g + 0.11*b;
    Uint32 color = SDL_MapRGB(format, average, average, average);
    return color;
}

void surfaceProcessing(SDL_Surface* surface, enum ProcessingType processingType)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;

    if(SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    Uint32 (*processor)(Uint32, SDL_PixelFormat*) = &pixelToGrayScale;
    switch (processingType) {
        case GRAYSCALE:
            processor = &pixelToGrayScale;
            break;
        case COLOR_INVERT:
            processor = &pixelInvert;
            break;
        case UP_CONTRAST:
            processor = &pixelBlackWhite;
            break;
    }

    for(int i = 0; i < len; i++)
        pixels[i] = (*processor)(pixels[i], format);

    SDL_UnlockSurface(surface);
}