//
// Created by quentin on 06/10/22.
//

#ifndef OCR_IMAGE_LOADING_H
#define OCR_IMAGE_LOADING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utils.h"
#include "image.h"
#include <sys/stat.h>

SDL_Surface* loadPngToSurface(const char* path);
Image getImageFromPng(const char* path);
void saveImageToBmp(Image* image, char* name, char* parent);
void saveSurfaceToBmp(SDL_Surface* surface, char* name, char* parent, char* format);
void saveImageToPdf(Image* image, char* name);

#endif
