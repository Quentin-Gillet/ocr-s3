//
// Created by quentin on 06/10/22.
//

#ifndef OCR_IMAGE_LOADING_H
#define OCR_IMAGE_LOADING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utils.h"
#include "image.h"

SDL_Surface* loadImage(const char* path);
void saveImageToBmp(Image* image, char* name);
void saveSurfaceToBmp(SDL_Surface* surface, char* name);

#endif
