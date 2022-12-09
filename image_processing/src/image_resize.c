//
// Created by Quentin Gillet on 07/12/2022.
//

#include "../include/image_resize.h"

Image imageResize(Image *image, unsigned int width, unsigned int height)
{
    double xScale = width / (double)image->width;
    double yScale = height / (double)image->height;

    Image newImage = createImage((int)width, (int)height);

    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            double oldX = x / xScale;
            double oldY = y / yScale;

            unsigned int top = floor(oldY);
            unsigned int bottom = top + 1;
            unsigned int left = floor(oldX);
            unsigned int right = left + 1;

            if (top < (unsigned int)image->height && left < (unsigned int)image->width && bottom < (unsigned int)image->height && right < (unsigned int)image->width)
            {
                double pixel = bilinearInterpolation(
                        top, bottom, right, left, oldX, oldY, image);

                setPixelSameValue(&(newImage.pixels[x][y]), pixel);
            }
        }
    }

    return newImage;
}

Image copyAndResizeImage(Image* image, unsigned int width, unsigned int height)
{
    int xDiff = floor((double)abs((int)width - image->width) / 2);
    int yDiff = floor((double)abs((int)height - image->height) / 2);

    Image cpImage = createImage((int)width, (int)height);
    for(int x = 0; x < image->width; x++)
    {
        for(int y = 0; y < image->height; y++)
        {
            cpImage.pixels[x + xDiff][y + yDiff] = copyPixel(&image->pixels[x][y]);
        }
    }
    return cpImage;
}