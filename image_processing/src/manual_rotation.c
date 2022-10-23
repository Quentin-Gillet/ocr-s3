#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


void surfaceManualRotation(SDL_Surface* surface, SDL_Renderer* renderer, long angle)
{
	if(SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopyEx(renderer, surface, NULL, NULL, angle, NULL, SDL_FLIP_NONE);
	SDL_RenderReadPixels(renderer, NULL, surface->format, surface->pixels, surface->pitch);

	SDL_UnlockSurface(surface);
}
