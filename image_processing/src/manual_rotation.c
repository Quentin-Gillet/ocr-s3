#include <SDL2/SDL.h>
#include <err.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>

Uint32 SDL_LirePixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;

	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp)
	{
		case 1:
			return *p;
		case 2:
			return *(Uint16 *)p;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
		case 4:
			return *(Uint32 *)p;
		default:
			return 0;
	}
}

void SDL_EcrirePixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;

	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp)
	{
		case 1:
			*p = pixel;
			break;
		case 2:
			*(Uint16 *)p = pixel;
			break;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			}
			else
			{
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;
		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}

SDL_Surface* surfaceManualRotation(SDL_Surface* origine, float angle)
{
	Uint32 couleur;
	int bx, by;

	float angle_radian = -angle * M_PI / 180.0;

	SDL_Surface* newSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, origine->w, origine->h, origine->format->BitsPerPixel,
			origine->format->Rmask, origine->format->Gmask, origine->format->Bmask, origine->format->Amask);
	if (newSurface == NULL)
		return NULL;

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
				couleur = SDL_LirePixel(origine, bx, by);
				SDL_EcrirePixel(newSurface, i, j, couleur);
			}
		}
	}

	return newSurface;
}
