#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>


// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* tSurface = IMG_Load(path);
    if(tSurface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(tSurface, SDL_PIXELFORMAT_RGB888, 0);
    if(surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(tSurface);
    return surface;
}

float truncate(float value, int val1, int val2)
{
    if(value < val1)
        return val1;
    if(value > val2)
        return val2;
    return value;
}

Uint32 pixel_invert(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    return SDL_MapRGB(format, 255 - r, 255 - g, 255 - b);
}

void surface_invert(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;

    if(SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for(int i = 0; i < len; i++)
        pixels[i] = pixel_invert(pixels[i], format);

    SDL_UnlockSurface(surface);
}

Uint32 pixel_up_contrast(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    int contrast_force = 100;
    float contrast_factor = (259 * (255 + contrast_force)) / (255 * (259 - contrast_force));
    r = truncate(contrast_factor * (r - 128) + 128, 0, 255);
    g = truncate(contrast_factor * (g - 128) + 128, 0, 255);
    b = truncate(contrast_factor * (b - 128) + 128, 0, 255);
    Uint32 color = SDL_MapRGB(format, r, g, b);
    return color;
}

void surface_up_contrast(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;

    if(SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for(int i = 0; i < len; i++)
        pixels[i] = pixel_up_contrast(pixels[i], format);

    SDL_UnlockSurface(surface);
}

// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint8 average = 0.3*r + 0.59*g + 0.11*b;
    Uint32 color = SDL_MapRGB(format, average, average, average);
    return color;
}

void surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;

    if(SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    for(int i = 0; i < len; i++)
        pixels[i] = pixel_to_grayscale(pixels[i], format);

    SDL_UnlockSurface(surface);
}

char* stradd(const char* a, const char* b){
    size_t len = strlen(a) + strlen(b);
    char *ret = (char*)malloc(len * sizeof(char) + 1);
    *ret = '\0';
    return strcat(strcat(ret, a) ,b);
}

void save_image(SDL_Surface* surface_image, char* name)
{
    char* file_name = stradd(name, "_.bmp");
    if(SDL_SaveBMP(surface_image, file_name) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    free(file_name);
}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

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

    SDL_Surface* surface = load_image(argv[1]);
    // Gets the width and the height of the texture.
    SDL_SetWindowSize(window, surface->w, surface->h);
    SDL_Texture* colored = SDL_CreateTextureFromSurface(renderer, surface);
    if(colored == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    surface_to_grayscale(surface);
    save_image(surface, "greyscale");

    surface_up_contrast(surface);
    save_image(surface, "contrast");

    surface_invert(surface);
    save_image(surface, "inverted");

    SDL_FreeSurface(surface);

    SDL_DestroyTexture(colored);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
