//
// Created by quentin on 06/10/22.
//

#include "../include/image_loading.h"

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

void saveImageToBmp(Image* image, char* name)
{
    SDL_Surface* surface = crateSurfaceFromImage(image);
    saveSurfaceToBmp(surface, name, "images/", ".bmp");
}

void saveSurfaceToBmp(SDL_Surface* surface, char* name, char* parent, char* format)
{
    char* imageFile = concat(parent, name);
    char* fileName = concat(imageFile, format);

    mkdir(parent, 0777);

    if(SDL_SaveBMP(surface, fileName) != 0)
        errx(EXIT_FAILURE, "saveImageToBmp: %s", SDL_GetError());
    free(fileName);
    free(imageFile);
    SDL_FreeSurface(surface);
}

void saveImageToPdf(Image* image, char* name)
{
    SDL_Surface* surface = crateSurfaceFromImage(image);
    saveSurfaceToBmp(surface, name, "", ".pdf");
}
