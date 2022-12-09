#include "../include/manual_rotation.h"


double bilinearInterpolation(unsigned int top, unsigned int bottom, unsigned int right, unsigned int left,
                             double horizontalPos, double verticalPos, Image* image)
{
    double topLeft = image->pixels[left][top].r;
    double topRight = image->pixels[right][top].r;
    double bottomLeft = image->pixels[left][bottom].r;
    double bottomRight = image->pixels[right][bottom].r;

    double horizontal_progress = horizontalPos - (double)left;
    double vertical_progress = verticalPos - (double)top;

    double top_block = topLeft + horizontal_progress * (topRight - topLeft);
    double bottom_block =
            bottomLeft + horizontal_progress * (bottomRight - bottomLeft);

    return top_block + vertical_progress * (bottom_block - top_block);
}

void imageRotate(Image* image, int angleDegree)
{
    const double middleX = (double)(image->width / 2.0);
    const double middleY = (double)(image->height / 2.0);

    const double angle = degToRad(angleDegree);

    Image newImage = copyImage(image);

    double newX, newY;
    unsigned int top, bottom, left, right;

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
            bottom = top + 1;
            left = floor(newX);
            right = left + 1;

            if(top < image->height && bottom < image->height && left < image->width && right < image->width)
            {
                unsigned int interpolate = bilinearInterpolation(top, bottom, right, left,
                                                        newX, newY, &newImage);
                setPixelSameValue(&image->pixels[x][y], interpolate);
            }
        }
    }

    freeImage(&newImage);
}