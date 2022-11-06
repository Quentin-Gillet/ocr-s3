//
// Created by Quentin Gillet on 26/10/2022.
//
#include "image.h"

/*
 * Get a pixel from x and y
 */
Uint32 getPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
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
            return 0;       /* shouldn't happen, but avoids warnings */
    }
}

/*
 * change pixel value on a x and y
 */
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
        case 1:
            *p = pixel;
            break;
        case 2:
            *(Uint16 *)p = pixel;
            break;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
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

Pixel copyPixel(Pixel* pixel)
{
    Pixel cpPixel = {pixel->r, pixel->g, pixel->b, pixel->pixelAverage};
    return cpPixel;
}

Image copyImage(Image* image)
{
    Pixel **pixels = NULL;
    pixels = calloc(image->width + 1, sizeof (Pixel *));
    for(int x = 0; x < image->width; x++)
    {
        pixels[x] = calloc(image->height + 1, sizeof (Pixel));
        for(int y = 0; y < image->height; y++)
        {
            pixels[x][y] = copyPixel(&image->pixels[x][y]);
        }
    }
    Image cpImage = {image->width, image->height, pixels};
    return cpImage;
}

/*
 * Transform a SDL_Surface into Image
 * with a double array pixels
 * width = surface->w
 * height = surface->h
 */
Image newImage(SDL_Surface* surface)
{
    const int surfaceHeight = surface->h;
    const int surfaceWidth = surface->w;
    Pixel **pixels = NULL;
    pixels = calloc(surfaceWidth + 1, sizeof(Pixel *));
    if (pixels == NULL)
    {
        errx(1, "Memory error");
    }
    for (int j = 0; j < surfaceWidth; j++)
    {
        pixels[j] = calloc(surfaceHeight + 1, sizeof(Pixel));
        if (pixels[j] == NULL)
        {
            errx(1, "Memory error");
        }
    }

    for(int x  = 0; x < surfaceWidth; x++)
    {
        for(int y = 0; y < surfaceHeight; y++)
        {
            Uint32 pixelAddr = getPixel(surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixelAddr, surface->format, &r, &g, &b);
            Pixel pixel = {r, g, b, (r + g + b) / 3};
            pixels[x][y] = pixel;
        }
    }

    Image image = {surfaceWidth, surfaceHeight, pixels};
    return image;
}

/*
 * Transform a Image into SDL_Surface
 */
SDL_Surface* crateSurfaceFromImage(Image* image)
{
    const int width = image->width;
    const int height = image->height;

    // Create rgb surface from image
    SDL_Surface *surface =
            SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

    // For each pixel in the source image
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            // Get pixel from image
            Pixel _pixel = image->pixels[x][y];

            // Get pixel value for SDL
            Uint32 pixel =
                    SDL_MapRGB(surface->format, _pixel.r, _pixel.g, _pixel.b);

            // Put pixel in img
            putPixel(surface, x, y, pixel);
        }
    }
    return surface;
}

/*
 * Free image for memory leaking
 */
void freeImage(Image* image)
{
    for(int x = 0; x < image->width; x++)
    {
        free(image->pixels[x]);
    }
    free(image->pixels);
}

/*
 * Set all color of a pixel into the same value
 */
void setPixelSameValue(Pixel* pixel, Uint8 value)
{
    pixel->r = value;
    pixel->g = value;
    pixel->b = value;

    pixel->pixelAverage = value;
}

/*
 * Set different color for each color of a pixel
 */
void setPixelValue(Pixel* pixel, Uint8 r, Uint8 g, Uint8 b)
{
    pixel->r = r;
    pixel->g = g;
    pixel->b = b;

    pixel->pixelAverage = (r + g + b) / 3;
}

/*
 * Recalculate pixel average when needed
 */
void calculatePixelAverage(Pixel* pixel)
{
    pixel->pixelAverage = (pixel->r + pixel->g + pixel->b) / 3;
}
