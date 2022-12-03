#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <../include/manual_rotation.h>
#include <../include/image_processing.h>

SDL_Surface* surfaceManualRotation(SDL_Surface* surface, SDL_Renderer* renderer, double angle)
{
	SDL_Texture* texture = 
		SDL_CreateTextureFromSurface(renderer, surface);

	SDL_RenderCopyEx(renderer, texture, NULL, NULL, angle, NULL,
		       	SDL_FLIP_NONE);
	
	SDL_RenderReadPixels(renderer, NULL, 
			surface->format->BitsPerPixel, surface->format->Rmask,
		    surface->format->Gmask, surface->format->Bmask,
			surface->format->Amask, surface->pixels,
			surface->pitch);
/*	Uint32 couleur;
	int bx, by;

	float angle_radian = -angle * M_PI / 180.0;

	SDL_Surface* newSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, origine->w, origine->h, origine->format->BitsPerPixel,
			origine->format->Rmask, origine->format->Gmask, origine->format->Bmask, origine->format->Amask);
	if (newSurface == NULL)		return NULL;

	int mx = origine->w / 2;
	int my = origine->h / 2;

	for(int i = 0; i < origine->h; i++)
	{
		for(int j = 0; j < origine->w; j++)
		{
			bx = (int) (cos(angle_radian) * (j-mx) + sin(angle_radian) * (i-my)) + mx;
			by = (int) (-sin(angle_radian) * (j-mx) + cos(angle_radian) * (i-my)) + my;

			if (bx >= 0 && bx < origine->w && by >= 0 && by < origine->h)
			{
				couleur = getPixel(origine, bx, by);
				putPixel (newSurface, i, j, couleur);
			}
		}
	}

	return newSurface;
	*/
}
