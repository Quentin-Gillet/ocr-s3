//
// Created by quentin on 06/10/22.
//

#ifndef OCR_IMAGE_LOADING_H
#define OCR_IMAGE_LOADING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utils.h"

SDL_Surface* loadPngImage(const char* path);
void saveImageToBmp(SDL_Surface* surface, char* name);

#endif
