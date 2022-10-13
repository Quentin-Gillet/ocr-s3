#include "../include/image_processing.h"

// Invert pixel color
Uint32 pixelInvert(Uint32 pixelColor, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixelColor, format, &r, &g, &b);
    return SDL_MapRGB(format, 255 - r, 255 - g, 255 - b);
}

Uint32 pixelBlackWhite(Uint32 pixelColor, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixelColor, format, &r, &g, &b);
    if((r + g + b) / 3 >= 127)
    {
        r = 255;
        g = 255;
        b = 255;    
    }
    else
    {
        r = 0;
        g = 0;
        b = 0;
    }
    Uint32 color = SDL_MapRGB(format, r, g, b);
    return color;
}

// Converts a colored pixel into grayscale.
//
// pixelColor: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixelToGrayScale(Uint32 pixelColor, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixelColor, format, &r, &g, &b);
    Uint8 average = 0.3*r + 0.59*g + 0.11*b;
    Uint32 color = SDL_MapRGB(format, average, average, average);
    return color;
}

void surfaceProcessing(SDL_Surface* surface, enum ProcessingType processingType)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;

    if(SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    Uint32 (*processor)(Uint32, SDL_PixelFormat*) = &pixelToGrayScale;
    switch (processingType) {
        case GRAYSCALE:
            processor = &pixelToGrayScale;
            break;
        case COLOR_INVERT:
            processor = &pixelInvert;
            break;
        case BLACK_WHITE:
            processor = &pixelBlackWhite;
            break;
        case UP_CONTRAST:
            processor = &pixelContrast;
            break;
    }

    for(int i = 0; i < len; i++)
        pixels[i] = (*processor)(pixels[i], format);

    SDL_UnlockSurface(surface);
}

Uint32 pixelContrast(Uint32 pixel_color, SDL_PixelFormat* format)
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

/*
Get pixel at a certain x and y
*/
Uint32 getPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

/*
Replace a pixel at an x and y
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

/*
Get adjacente pixels of given coords
*/
Uint32* getAdjacentPixels(SDL_Surface* surface, int x, int y)
{
    int possibleCoords[] = {-1, 0, 1};
    static Uint32 pixels[9];
    int index = 0;

    for(int j = 0; j < 3; j++)
    {
        for(int i = 0; i < 3; i++)
        {
            if(possibleCoords[j] == 0 && possibleCoords[i] == 0)
                continue;
            pixels[index] = getPixel(surface, x + possibleCoords[j], y + possibleCoords[i]);
            index++;
        }
    }
    pixels[8] = getPixel(surface, x, y);
    return pixels;
}

int otsuMethod(SDL_Surface* surface)
{
    long totalPixels = surface->w * surface->h;

    float var_max = 0, sum = 0, sumB = 0;
    int threshold = 0, q1 = 0, q2 = 0;
    int max_intensity = 255;
    unsigned histogram[256] = {0};

    for(int i = 0; i < totalPixels; i++)
    {
        Uint32* pixels = surface->pixels;
        Uint32 pixel = pixels[i];
        Uint8 r, g, b;
        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
        int pixel_value = (int)truncate((r + g + b) / 3, 0, 255);
        histogram[pixel_value]++;
    }

    for(int i = 0; i <= max_intensity; i++)
    {
        sum += i * (int)histogram[i];
    }

    for(int t = 0; t <= max_intensity; t++)
    {
        q1 += histogram[t];
        if(q1 == 0)
            continue;
        q2 = totalPixels - q1;

        if(q2 == 0)
            break;

        sumB += (float)(t * histogram[t]);
        float u1 = sumB / q1;
        float u2 = (sum - sumB) / q2;

        float otsu = (float)q1 * (float)q2 * (u1 - u2) * (u1 - u2);
        if(otsu > var_max)
        {
            threshold = t;
            var_max = otsu;
        }
    }

    return threshold;
}

void surfaceBinarisaion(SDL_Surface* surface)
{
    if(SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    int x, y;
    int meanIntensity = otsuMethod(surface);

    Uint8 r, g, b;
    for(x = 0; x < surface->w; x++)
    {
        for(y = 0; y < surface->h; y++)
        {
            Uint32 pixel = getPixel(surface, x, y);
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            long intensity = (r + g + b) / 3;
            Uint32 finalPixel;
            if(intensity > meanIntensity)
                finalPixel = SDL_MapRGB(surface->format, 255, 255, 255);
            else
                finalPixel = SDL_MapRGB(surface->format, 0, 0, 0);
            putPixel(surface, x, y, finalPixel);
        }
    }

    SDL_UnlockSurface(surface);
} 

/*
Sort pixels list and get the median value
To perform good sorting with no color error we need to transform RGB to HSV
And after the sort retransform back HSV to RGB
*/
Uint32 getPixelMedian(SDL_PixelFormat* format, Uint32* pixels)
{
    double hue[9];
    double sat[9];
    double value[9];

    Uint8 r, g, b;
    for(int i = 0; i < 9; i++)
    {
        SDL_GetRGB(pixels[i], format, &r, &g, &b);
        rgb color = {r / 255., g / 255., b / 255.};
        hsv colorHsv = rgb2hsv(color);
        hue[i] = colorHsv.h;
        sat[i] = colorHsv.s;
        value[i] = colorHsv.v;
    }

    insertionSort(hue, 9);
    insertionSort(sat, 9);
    insertionSort(value, 9);

    hsv colorHsv = {hue[4], sat[4], value[4]};
    rgb color = hsv2rgb(colorHsv);

    return SDL_MapRGB(format, color.r * 255, color.g * 255, color.b * 255);
}

/*
Median filter algorithm apply on surface
*/
void surfaceMedianBlur(SDL_Surface* surface)
{
    if(SDL_LockSurface(surface) < 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    int x, y;

    for(x = 1; x < surface->w - 1; x++)
    {
        for(y = 1; y < surface->h - 1; y++)
        {
            Uint32* adjacentsPixels = getAdjacentPixels(surface, x, y);
            Uint32 pixel = getPixelMedian(surface->format, adjacentsPixels);
            putPixel(surface, x, y, pixel);
        }
    }

    SDL_UnlockSurface(surface);
}

double convolution(SDL_Surface* surface, double kernel[3][3], int row, int col)
{
    double sum = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int x = i + row;
            int y = j + col;
            if (x >= 0 && y >= 0 && x < (int)surface->w
                && y < (int)surface->h)
            {
                Uint32 pixel = getPixel(surface, x, y);
                Uint8 rt, rg, rb;
                SDL_GetRGB(pixel, surface->format, &rt, &rg, &rb);
                sum += rt * kernel[i][j];
            }
        }
    }

    return sum;
}

void surfaceSobelFilter(SDL_Surface* surface)
{
    double gx, gy;
    double g_px;

    double kernel_x[3][3] = { { -1.0, 0.0, 1.0 },
                              { -2.0, 0.0, 2.0 },
                              { -1.0, 0.0, 1.0 } };

    double kernel_y[3][3] = { { -1.0, -2.0, -1.0 },
                              { 0.0, 0.0, 0.0 },
                              { 1.0, 2.0, 1.0 } };

    const unsigned int height = surface->h;
    const unsigned int width = surface->w;
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            gx = convolution(surface, kernel_x, j, i);
            gy = convolution(surface, kernel_y, j, i);
            g_px = sqrt(gx * gx + gy * gy);

            Uint8 color = (unsigned int)(g_px > 128 ? 255 : 0);
            Uint32 pixel = SDL_MapRGB(surface->format, color, color, color);

            putPixel(surface, j, i, pixel);
        }
    }
}