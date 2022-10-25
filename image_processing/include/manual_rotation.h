#ifndef OCR_MANUAL_ROTATION_H
#define OCR_MANUAL_ROTATION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "utils.h"

void surfaceManualRotation(SDL_Surface* surface, SDL_Renderer* renderer, int angle);

#endif
