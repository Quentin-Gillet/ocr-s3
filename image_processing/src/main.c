#include "../include/image_processing.h"
#include "../include/image_loading.h"
#include "../include/manual_rotation.h"

void clearSdl(SDL_Renderer* renderer, SDL_Window* window)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2 && argc != 3)
        errx(EXIT_FAILURE, "Usage: image-file (+ rotation)");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Image", 0, 0, 0, 0,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* surface = loadPngImage(argv[1]);

    // Gets the width and the height of the texture.
    SDL_SetWindowSize(window, surface->w, surface->h);

    surfaceProcessing(surface, GRAYSCALE);
    saveImageToBmp(surface, "greyscale");

    surfaceMedianBlur(surface);
    saveImageToBmp(surface, "blur");

    // surfaceProcessing(surface, BLACK_WHITE);
    // saveImageToBmp(surface, "blackwhite");

    surfaceBinarisaion(surface);
    saveImageToBmp(surface, "mean");

    surfaceProcessing(surface, COLOR_INVERT);
    saveImageToBmp(surface, "inverted");

    surfaceSobelFilter(surface);
    saveImageToBmp(surface, "sobel");

    if(argc == 3)
    {
	    surfaceManualRotation(surface, renderer, argv[2]);
	    saveImageToBmp(surface, "rotated");
    }

    SDL_FreeSurface(surface);

    clearSdl(renderer, window);
    return EXIT_SUCCESS;
}
