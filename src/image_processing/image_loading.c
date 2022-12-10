//
// Created by quentin on 06/10/22.
//

#include "image_processing/image_loading.h"

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* loadPngToSurface(const char* path)
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

Image getImageFromPng(const char* path)
{
    SDL_Surface* surface = loadPngToSurface(path);
    Image image = createImageFromSurface(surface);
    SDL_FreeSurface(surface);

    return image;
}

void saveImageToBmp(Image* image, char* name, char* parent)
{
    SDL_Surface* surface = crateSurfaceFromImage(image);
    char path[100];
    if(strcmp("", parent))
    {
        snprintf(path, 100, "images/%s/", parent);
    }
    saveSurfaceToBmp(surface, name, !strcmp("", parent) ? "images/" : path, ".bmp");
}

void saveSurfaceToBmp(SDL_Surface* surface, char* name, char* parent, char* format)
{
    char fileName[100];
    snprintf(fileName, 100, "%s%s%s", parent, name, format);

    mkdir(parent, 0777);

    if(SDL_SaveBMP(surface, fileName) != 0)
        errx(EXIT_FAILURE, "saveImageToBmp: %s", SDL_GetError());
    SDL_FreeSurface(surface);
}

void saveImageToPdf(Image* image, char* name)
{
    SDL_Surface* surface = crateSurfaceFromImage(image);
    saveSurfaceToBmp(surface, name, "", ".pdf");
}
