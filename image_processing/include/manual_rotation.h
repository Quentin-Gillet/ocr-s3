#ifndef OCR_MANUAL_ROTATION_H
#define OCR_MANUAL_ROTATION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utils.h"

Uint32 SDL_LirePixel(SDL_Surface* surface, int x, int y);
void SDL_EcrirePixel(SDL_Surface* surface, int x, int y, Uint32 pixel);
SDL_Surface* surfaceManualRotation(SDL_Surface* origine, float angle);

#endif
