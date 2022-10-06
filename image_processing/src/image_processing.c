#include "../include/image_processing.h"

// Invert pixel color
Uint32 pixelInvert(Uint32 pixelColor, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixelColor, format, &r, &g, &b);
    return SDL_MapRGB(format, 255 - r, 255 - g, 255 - b);
}

Uint32 pixelUpContrast(Uint32 pixelColor, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixelColor, format, &r, &g, &b);
    int contrast_force = 200;
    float contrast_factor = (259 * (255 + contrast_force)) / (255 * (259 - contrast_force));
    r = truncate(contrast_factor * (r - 128) + 128, 0, 255);
    g = truncate(contrast_factor * (g - 128) + 128, 0, 255);
    b = truncate(contrast_factor * (b - 128) + 128, 0, 255);
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
            processor = &pixelUpContrast;
            break;
    }

    for(int i = 0; i < len; i++)
        pixels[i] = (*processor)(pixels[i], format);

    SDL_UnlockSurface(surface);
}