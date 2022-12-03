//
// Created by quentin on 06/10/22.
//

#include "../include/image_loading.h"

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* loadPngImage(const char* path)
{
    SDL_Surface* tSurface = IMG_Load(path);
    if(tSurface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tSurface, SDL_PIXELFORMAT_RGB888, 0);
    if(surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(tSurface);
    return surface;
}

void saveImageToBmp(SDL_Surface* surface, char* name)
{
    char* fileName = stradd(name, "_.bmp");
    if(SDL_SaveBMP(surface, fileName) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    free(fileName);
}
