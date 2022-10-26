#include "../include/image_processing.h"

// Converts a image into grayscale image.
void imageGrayscale(Image* image)
{
    for(size_t x = 0; x < image->width; x++)
    {
        for(size_t y = 0; y < image->height; y ++)
        {
            Uint8 average = clamp(0.3 * image->pixels[x][y].r
                                  + 0.59 * image->pixels[x][y].g
                                  + 0.11 * image->pixels[x][y].b, 0, 255);
            setPixelSameValue(&image->pixels[x][y], average);
        }
    }
}

// Invert image color
void imageInvert(Image* image)
{
    for(size_t x = 0; x < image->width; x++)
    {
        for(size_t y = 0; y < image->height; y ++)
        {
            setPixelValue(&image->pixels[x][y], 255 - image->pixels[x][y].r,
                                                    255 - image->pixels[x][y].g,
                                                    255 - image->pixels[x][y].g);
        }
    }
}

unsigned int maxColor(Image* image)
{
    unsigned int m = 0;
    for(size_t x = 0; x < image->width; x++)
        for(size_t y = 0; y < image->height; y++)
            if(image->pixels[x][y].r > m)
                m = image->pixels[x][y].r;
    return m;
}

void normalizeBrightness(Image* image)
{
    int m  = maxColor(image);
    for(size_t x  = 0; x < image->width; x++)
    {
        for(size_t y = 0; y < image->height; y++)
        {
            int val = 255 - image->pixels[x][y].r * (255 / m);
            setPixelSameValue(&image->pixels[x][y], val);
        }
    }
}

void imageContrastFilter(Image* image)
{
    for(size_t x  = 0; x < image->width; x++)
    {
        for(size_t y = 0; y < image->height; y++)
        {
            int r = image->pixels[x][y].r;
            for(int i = 0; i < 10; i++)
            {
                if(r >= i * (255 / 10) && r <= (i + 1) * (255 / 10))
                {
                    int v = (i + 1) * (255 / 10);
                    setPixelSameValue(&image->pixels[x][y], v);
                }
            }
        }
    }

    imageInvert(image);
    normalizeBrightness(image);
}

void imageBrightness(Image* image, int brightness)
{
    for(size_t x = 0; x < image->width; x++)
    {
        for(size_t y = 0; y < image->height; y++)
        {
            Uint8 val =
                    clampUint8((image->pixels[x][y].r + brightness), 0, 255);
            setPixelSameValue(&image->pixels[x][y], val);
        }
    }
}

/*
Get adjacent pixels of given coords
*/
Pixel* getAdjacentPixels(Image* image, int x, int y)
{
    int possibleCoords[] = {-1, 0, 1};
    static Pixel pixels[9];
    int index = 0;

    for(int j = 0; j < 3; j++)
    {
        for(int i = 0; i < 3; i++)
        {
            pixels[index] = image->pixels[x + possibleCoords[j]][y + possibleCoords[i]];
            index++;
        }
    }
    return pixels;
}

int* getImageHistogram(Image* image)
{
    int *histogram = calloc(256, sizeof(int));

    for(size_t x = 0; x < image->width; x++)
    {
        for(size_t y = 0; y < image->height; y++)
        {
            histogram[image->pixels[x][y].pixelAverage]++;
        }
    }

    return histogram;
}

int otsuMethod(Image* image)
{
    long totalPixels = image->width * image->height;

    float var_max = 0, sum = 0, sumB = 0;
    int threshold = 0, q1 = 0, q2 = 0;
    int max_intensity = 255;

    int *histogram = getImageHistogram(image);

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

void imageBinarization(Image* image)
{
    int meanIntensity = otsuMethod(image);

    for(size_t x = 0; x < image->width; x++)
    {
        for(size_t y = 0; y < image->height; y++)
        {
            if(image->pixels[x][y].pixelAverage > meanIntensity)
                setPixelSameValue(&image->pixels[x][y], 255);
            else
                setPixelSameValue(&image->pixels[x][y], 0);
        }
    }
} 

/*
Sort pixels list and get the median value
To perform good sorting with no color error we need to transform RGB to HSV
And after the sort transform back HSV to RGB
*/
Pixel getPixelMedian(Pixel* pixels)
{
    double hue[9];
    double sat[9];
    double value[9];

    for(int i = 0; i < 9; i++)
    {
        rgb color = {pixels[i].r / 255., pixels[i].g / 255., pixels[i].b / 255.};
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

    Uint8 r = color.r * 255;
    Uint8 g = color.g * 255;
    Uint8 b = color.b * 255;
    Uint8 average = (r + g + b) / 3;

    Pixel pixel = {r, g, b, average};

    return pixel;
}

/*
Median filter algorithm apply on surface
*/
void imageMedianBlur(Image* image)
{
    for(size_t x = 1; x < image->width - 1; x++)
    {
        for(size_t y = 1; y < image->height - 1; y++)
        {
            Pixel* adjacentPixels = getAdjacentPixels(image, x, y);
            Pixel pixel = getPixelMedian(adjacentPixels);
            image->pixels[x][y] = pixel;
        }
    }
}

double convolution(Image* image, double kernel[3][3], int row, int col)
{
    double sum = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int x = i + row;
            int y = j + col;
            if (x >= 0 && y >= 0 && x < (int)image->width
                && y < (int)image->height)
            {
                sum += image->pixels[x][y].r * kernel[i][j];
            }
        }
    }

    return sum;
}

void imageSobelFilter(Image* image)
{
    double gx, gy;
    double g_px;

    double kernel_x[3][3] = { { -1.0, 0.0, 1.0 },
                              { -2.0, 0.0, 2.0 },
                              { -1.0, 0.0, 1.0 } };

    double kernel_y[3][3] = { { -1.0, -2.0, -1.0 },
                              { 0.0, 0.0, 0.0 },
                              { 1.0, 2.0, 1.0 } };

    const unsigned int height = image->height;
    const unsigned int width = image->width;
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            gx = convolution(image, kernel_x, j, i);
            gy = convolution(image, kernel_y, j, i);
            g_px = sqrt(gx * gx + gy * gy);

            Uint8 color = (unsigned int)(g_px > 128 ? 255 : 0);
            setPixelSameValue(&image->pixels[j][i], color);
        }
    }
}