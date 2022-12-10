#include "image_processing/image_rotation.h"


void getMaxSize(Image* image, int angleDegree, int *maxTop, int *maxLeft, int *minTop, int *minLeft)
{
    const double middleX = (double)(image->width / 2.0);
    const double middleY = (double)(image->height / 2.0);

    const double angle = degToRad(angleDegree);

    double newY, newX;
    int top, left;

    for(int x = 0; x < image->width; x++)
    {
        for(int y = 0; y < image->height; y++)
        {
            newX = ((double)(cos(angle) * ((double)x - middleX)
                             - sin(angle) * ((double)y - middleY))
                    + middleX);
            newY = ((double)(cos(angle) * ((double)y - middleY)
                             + sin(angle) * ((double)x - middleX))
                    + middleY);

            top = floor(newY);
            left = floor(newX);

            if(top > *maxTop)
                *maxTop = top;
            if(left > *maxLeft)
                *maxLeft = left;
            if(top < *minTop)
                *minTop = top;
            if(left < *minLeft)
                *minLeft = left;
        }
    }

    if(*maxLeft < image->width)
        *maxLeft = image->width;
    if(*maxTop < image->height)
        *maxTop = image->height;

    if(*minLeft > 0)
        *minLeft = 0;
    if(*minTop > 0)
        *minTop = 0;

}

Image imageRotate(Image* image, int angleDegree)
{
    const double angle = degToRad(angleDegree);

    int maxTop = 0, maxLeft = 0, minTop = 0, minLeft = 0;
    getMaxSize(image, angleDegree, &maxTop, &maxLeft, &minTop, &minLeft);

    int toUpWidth = clampIntMin(maxTop + (-minTop), 0) + 1;
    int toUpHeight = clampIntMin(maxLeft + (-minLeft), 0);

    Image resizeImage = createImage(toUpWidth, toUpHeight);
    Image newImage = copyAndResizeImage(image, toUpWidth, toUpHeight);

    const double middleX = (double)(resizeImage.width / 2.0);
    const double middleY = (double)(resizeImage.height / 2.0);

    double newX, newY;
    unsigned int top, bottom, left, right;

    for(int x = 0; x < resizeImage.width; x++)
    {
        for(int y = 0; y < resizeImage.height; y++)
        {
            newX = ((double)(cos(angle) * ((double)x - middleX)
                             - sin(angle) * ((double)y - middleY))
                    + middleX);
            newY = ((double)(cos(angle) * ((double)y - middleY)
                             + sin(angle) * ((double)x - middleX))
                    + middleY);

            top = floor(newY);
            bottom = top + 1;
            left = floor(newX);
            right = left + 1;

            if(top < (unsigned int)resizeImage.height && bottom < (unsigned int)resizeImage.height && left < (unsigned int)resizeImage.width && right < (unsigned int)resizeImage.width)
            {
                unsigned int interpolate = (unsigned int)bilinearInterpolation(top, bottom, right, left,
                                                                 newX, newY, &newImage);
                setPixelSameValue(&resizeImage.pixels[x][y], interpolate);
            }
        }
    }

    freeImage(&newImage);
    freeImage(image);
    return resizeImage;
}