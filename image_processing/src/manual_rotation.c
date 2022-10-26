#include "manual_rotation.h"


void surfaceManualRotation(SDL_Surface* surface, SDL_Renderer* renderer, int angle)
{
    angle = clamp(angle, -360, 360);
	if(SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopyEx(renderer, texture, NULL, NULL, angle, NULL, SDL_FLIP_NONE);
	SDL_RenderReadPixels(renderer, NULL, 0, surface->pixels, surface->pitch);

	SDL_UnlockSurface(surface);
}
