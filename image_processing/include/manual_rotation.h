#ifndef OCR_MANUAL_ROTATION_H
#define OCR_MANUAL_ROTATION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

SDL_Surface* surfaceManualRotation(SDL_Surface* surface, SDL_Renderer* renderer, long angle);

#endif
