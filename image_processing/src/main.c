#include "../include/image_processing.h"
#include "../include/image_loading.h"
#include "../include/manual_rotation.h"
#include "../include/line_detection.h"

void clearSdl(SDL_Renderer* renderer, SDL_Window* window)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void pollEvent()
{
    SDL_Event e;
    while (1){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                return;
            }
        }
    }
}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2 && argc != 3)
        errx(EXIT_FAILURE, "Usage: image-file (+ rotation)");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500,
                                          SDL_WINDOW_SHOWN);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* surface = loadPngImage(argv[1]);

    // Gets the width and the height of the texture.
    //SDL_SetWindowSize(window, surface->w, surface->h);

    surfaceProcessing(surface, GRAYSCALE);
    saveImageToBmp(surface, "greyscale");

    surfaceContrastFilter(surface);
    saveImageToBmp(surface, "contrast");

    surfaceMedianBlur(surface);
    saveImageToBmp(surface, "blur");

    surfaceUpBrightness(surface, -30);
    saveImageToBmp(surface, "brightness");

    surfaceBinarisaion(surface);
    saveImageToBmp(surface, "mean");

    surfaceProcessing(surface, COLOR_INVERT);
    saveImageToBmp(surface, "inverted");

    surfaceSobelFilter(surface);
    saveImageToBmp(surface, "sobel");

    int linesLength = 0;
    struct Line* lines = houghTransform(surface, 0.4, &linesLength);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for(int i = 0; i < linesLength; i++)
    {
        struct Line line = lines[i];
        SDL_RenderDrawLine(renderer, line.x1, line.y1, line.x2, line.y2);
    }
    SDL_RenderPresent(renderer);

    // if(argc == 3)
    // {
	//     surfaceManualRotation(surface, 90);
	//     saveImageToBmp(surface, "rotated");
    // }

    pollEvent();
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    clearSdl(renderer, window);
    return EXIT_SUCCESS;
}
