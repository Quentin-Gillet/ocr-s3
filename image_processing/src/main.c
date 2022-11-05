#include "../include/image_processing.h"
#include "../include/image_loading.h"
#include "../include/manual_rotation.h"
#include "../include/line_detection.h"
#include "image.h"

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
    if (argc < 2 || argc > 3)
        errx(EXIT_FAILURE, "Usage: image-file (+ rotation)");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0,
                                          SDL_WINDOW_SHOWN);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* surface = loadImage(argv[1]);
    Image image = newImage(surface);
    SDL_FreeSurface(surface);

    // Gets the width and the height of the texture.
    SDL_SetWindowSize(window, (int)image.width, (int)image.height);

    imageGrayscale(&image);
    saveImageToBmp(&image, "greyscale");

    imageContrastFilter(&image);
    saveImageToBmp(&image, "contrast");

    imageMedianBlur(&image);
    saveImageToBmp(&image, "blur");

    imageBrightness(&image, -30);
    saveImageToBmp(&image, "brightness");

    imageBinarization(&image);
    saveImageToBmp(&image, "mean");

    imageInvert(&image);
    saveImageToBmp(&image, "inverted");

    imageSobelFilter(&image);
    saveImageToBmp(&image, "sobel");

    if (argc == 3)
    {
        surfaceManualRotation(surface, renderer, atoi(argv[2]));
        saveImageToBmp(&image, "rotated");
    }

    surface = crateSurfaceFromImage(&image);

    int linesLength = 0;
    struct Line* lines = houghTransform(&image, 0.5, &linesLength);

    drawLinesOnSurface(renderer, surface, lines, linesLength);
    saveSurfaceToBmp(surface, "hough");

    SDL_FreeSurface(surface);
    freeImage(&image);

    clearSdl(renderer, window);
    return EXIT_SUCCESS;
}
